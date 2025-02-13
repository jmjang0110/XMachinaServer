#include "pch.h"
#include "Session.h"
#include "PacketSendBuf.h"
#include "PacketRecvBuf.h"
#include "NetworkManager.h"
#include "SocketData.h"
#include "NetworkInterface.h"

#include "ThreadManager.h"
#include "Lock.h"



Session::Session() : NetworkObject()
{
	mPacketBuffer.RecvPkt = new PacketRecvBuf(static_cast<INT32>(PacketRecvBuf::Info::Size));
	
	SocketData sockdata = {};
	sockdata.CreateSocket();
	NetworkObject::SetSocketData(sockdata);

	NetworkObject::mID = static_cast<UINT32>(sockdata.GetSocket());

}

Session::~Session()
{
	LOG_MGR->Cout("~Session : ", mID, "\n");

	SAFE_DELETE(mPacketBuffer.RecvPkt);

}

void Session::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	/* What is the Type of Task? */
	OverlappedIO::Type IoType = overlapped->GetIoType();

	/* Process Task! */
	ProcessIO(IoType, bytes);
}

void Session::RegisterIO(OverlappedIO::Type IoType)
{
	switch (IoType)
	{
		/// +-------------------
		///	  REGISTER CONNECT
		/// -------------------+
	case OverlappedIO::Type::Connect:
	{
		if (mIsConnected.load() == true)
			return;

		GetSocketData().SetReuseAddress(true);
		GetSocketData().BindAnyAddress(0);

		/* Clear & Set - Connect Overlapped IO Object */
		mOverlapped.Connect.Clear_OVERLAPPED();
		mOverlapped.Connect.SetOwner(shared_from_this());

		/* Register Connect I/O */
		DWORD numOfBytes = {};
		bool ConnectExResult = NETWORK_MGR->ConnectEx()(GetSocketData().GetSocket()
										, reinterpret_cast<SOCKADDR*>(&GetSocketData().GetSockAddr())
										, sizeof(GetSocketData().GetSockAddr())
										, nullptr
										, 0
										, &numOfBytes
										, &mOverlapped.Connect);
		
		if (ConnectExResult == false) {
			INT32 errCode = ::WSAGetLastError();
			if (errCode != WSA_IO_PENDING) {
				mOverlapped.Connect.SetOwner(nullptr);
			}
		}
	}
		break;
		/// +-------------------
		///	REGISTER DISCONNECT
		/// -------------------+
	case OverlappedIO::Type::DisConnect:
	{
		mOverlapped.Disconnect.Clear_OVERLAPPED();
		mOverlapped.Disconnect.SetOwner(shared_from_this());

		bool DisconnectExResult = NETWORK_MGR->DiscconectEx()(GetSocketData().GetSocket()
															, &mOverlapped.Disconnect
															, TF_REUSE_SOCKET /* 소켓이 종료되어도 해당 주소와 포트를 다른 소켓이 사용가능 */
															, 0);

		if (DisconnectExResult == false) {
			INT32 errCode = ::WSAGetLastError();
			if (errCode != WSA_IO_PENDING) {
				mOverlapped.Disconnect.SetOwner(nullptr);
			}
		}
	}
		break;
		/// +-------------------
		///	   REGISTER SEND 
		/// -------------------+
	case OverlappedIO::Type::Send:
	{
		if (mIsConnected.load() == false) {
			return;
		}

		mOverlapped.Send.Clear_OVERLAPPED();
		mOverlapped.Send.SetOwner(shared_from_this());

		/* Push SendPkt to Overlapped_Send */
		{
			//WRITE_LOCK_SCOPE
			mSRWLock_AccessToSendPktQ.LockWrite();


			while (mPacketBuffer.SendPkt_Queue.empty() == false) {
				SPtr_SendPktBuf sendPktBuf = mPacketBuffer.SendPkt_Queue.front();
				mPacketBuffer.SendPkt_Queue.pop();
				if(sendPktBuf)
					mOverlapped.Send.BufPush(sendPktBuf);
			}

			mSRWLock_AccessToSendPktQ.UnlockWrite();
		}

		/* Scatter-Gather */
		std::vector<WSABUF> wsaBufs = { };
		wsaBufs.reserve(mOverlapped.Send.BufSize());
		for (SPtr_SendPktBuf sendBuf : mOverlapped.Send.GetSendBuffers()) {
		
			WSABUF wsaBuf = {};
			wsaBuf.buf    = reinterpret_cast<char*>(sendBuf->GetBuffer());
			wsaBuf.len    = static_cast<LONG>(sendBuf->GetTotalSize());
			wsaBufs.push_back(wsaBuf);
		}

		/* ::WSASend */
		DWORD numOfBytes = 0;
		if (SOCKET_ERROR == ::WSASend(GetSocketData().GetSocket()
									, wsaBufs.data()
									, static_cast<DWORD>(wsaBufs.size())
									, OUT & numOfBytes
									, 0
									, &mOverlapped.Send
									, nullptr))
		{
			INT32 errCode = ::WSAGetLastError();
			if (errCode != WSA_IO_PENDING)
			{
				ProcessError(errCode);
				mOverlapped.Send.DecRef_NetObj();
				mPacketBuffer.IsSendRegistered.store(false);
			}
		}

	}
		break;
		/// +-------------------
		///    REGISTER RECV
		/// -------------------+
	case OverlappedIO::Type::Recv:
	{
		if (mIsConnected.load() == false) {
			return;
		}
		mOverlapped.Recv.Clear_OVERLAPPED();
		mOverlapped.Recv.SetOwner(shared_from_this());

		/* ::WSARecv */
		WSABUF wsaBuf{};
		wsaBuf.buf = reinterpret_cast<char*>(mPacketBuffer.RecvPkt->GetWritePointer());
		wsaBuf.len = mPacketBuffer.RecvPkt->GetFreeSize();

		DWORD numOfBytes = 0;
		DWORD flags      = 0;
		int WSARecvResult = ::WSARecv(GetSocketData().GetSocket()
									, &wsaBuf
									, 1
									, &numOfBytes
									, &flags
									, &mOverlapped.Recv
									, nullptr);

		if (WSARecvResult == SOCKET_ERROR) {
			INT32 errCode = ::WSAGetLastError();
			if (errCode != WSA_IO_PENDING) {
				ProcessError(errCode);
				mOverlapped.Recv.SetOwner(nullptr);
			}
		}
	}
		break;
	}

}

void Session::ProcessIO(OverlappedIO::Type IoType, INT32 BytesTransferred)
{
	switch (IoType)
	{
	/// +-------------------
	///    PROCESS CONNECT
	/// -------------------+
	case OverlappedIO::Type::Connect:
	{
		mOverlapped.Connect.DecRef_NetObj(); // Shared_ptr -> release
		mIsConnected.store(true);

		UINT32 sessionID = static_cast<UINT32>(GetSocketData().GetSocket());
		GetOwnerNI()->AddSession(sessionID, std::static_pointer_cast<Session>(shared_from_this()));

		/* On Connect */
		OnConnected();

		/* Register Recv */
		RegisterIO(OverlappedIO::Type::Recv);

	}
	break;
	/// +-------------------
	///  PROCESS DISCONNECT
	/// -------------------+
	case OverlappedIO::Type::DisConnect:
	{
		mOverlapped.Disconnect.DecRef_NetObj(); // Shared_ptr -> release 

		OnDisconnected();
		GetOwnerNI()->DeleteSession(NetworkObject::mID); /* ID : protected */

	}
	break;
	/// +-------------------
	///    PROCESS SEND
	/// -------------------+
	case OverlappedIO::Type::Send:
	{

		mOverlapped.Send.DecRef_NetObj(); 
		mOverlapped.Send.ReleaseSendBuffersReferenceCount(); /* SendBuffers Send Complete → Release Reference Count */

		/* Exception Disconnect */
		if (BytesTransferred == 0) {
			Disconnect(L"Send 0 - Disconnect!");
			break;
		}

		OnSend(BytesTransferred);

		/* 다 보냄 */
		{
			mSRWLock_AccessToSendPktQ.LockWrite();

			if (mPacketBuffer.SendPkt_Queue.empty() == true) {
				mPacketBuffer.IsSendRegistered.store(false);
			}

			/* 다 안보냄 */
			bool PktQ_IsEmpty = mPacketBuffer.SendPkt_Queue.empty();
			mSRWLock_AccessToSendPktQ.UnlockWrite();

			if (PktQ_IsEmpty == false) {
				RegisterIO(OverlappedIO::Type::Send); // Lock Write 
			}
		}


	}
	break;
	/// +-------------------
	///    PROCESS RECV
	/// -------------------+
	case OverlappedIO::Type::Recv:
	{
		mOverlapped.Recv.DecRef_NetObj();

		/* Exception Disconnect */
		if (BytesTransferred == 0) {
			Disconnect(L"Recv 0 - Disconnect!");
			break;
		}
		if (FALSE == mPacketBuffer.RecvPkt->OnWrite(BytesTransferred)) {
			Disconnect(L"OnWirte Overflow - Disconnect!");
			break;
		}

		/* On Recv */
		UINT32 DataSize = mPacketBuffer.RecvPkt->GetDataSize();
		UINT32 ProcessLen = OnRecv(mPacketBuffer.RecvPkt->GetReadPointer(), DataSize);
		if (FALSE == (ProcessLen < 0 || DataSize < ProcessLen || mPacketBuffer.RecvPkt->OnRead(ProcessLen))) {
			Disconnect(L"OnRead Overflow - Disconnect!");
			break;
		}

		/* Register IO */
		mPacketBuffer.RecvPkt->Clean();
		RegisterIO(OverlappedIO::Type::Recv);

		break;
	}
	}
}
void Session::ProcessError(INT32 errCode)
{
	switch (errCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TODO : Log
		std::cout << "ProcessError : " << errCode << std::endl;
		break;
	}
}

void Session::Send(SPtr_SendPktBuf buf)
{
	if (mIsConnected.load() == false) {
		return;
	}

	bool RegisterSend = false;
	{
		//WRITE_LOCK_SCOPE;

		mSRWLock_AccessToSendPktQ.LockWrite();

		mPacketBuffer.SendPkt_Queue.push(buf);

		if (mPacketBuffer.IsSendRegistered.exchange(true) == false)
			RegisterSend = true;

		mSRWLock_AccessToSendPktQ.UnlockWrite();

	}

	if (RegisterSend) {
		RegisterIO(OverlappedIO::Type::Send);
	}

}

void Session::Connect()
{
	RegisterIO(OverlappedIO::Type::Connect);
}

void Session::Disconnect(const WCHAR* cause)
{
	if (mIsConnected.exchange(false) == false)
		return;	
	std::wcout << "Disconnect : " << cause << std::endl;

	RegisterIO(OverlappedIO::Type::DisConnect);
}


void Session::SetIpPort(std::wstring ip, UINT32 port)
{
	SOCKADDR_IN SockAddr{};
	/* Socket Address */
	::memset(&SockAddr, 0, sizeof(SockAddr));

	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr = NETWORK_MGR->Ip2Address(ip.c_str());
	SockAddr.sin_port = ::htons(port); // 포트 번호 : 16비트 숫자-host to Network - short(16bit)

	int id = GetID();

	GetSocketData().SetSockAddrIn(SockAddr);
	SocketData data = GetSocketData();

}
