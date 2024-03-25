#include "pch.h"
#include "Session.h"
#include "PacketSendBuf.h"
#include "PacketRecvBuf.h"
#include "NetworkManager.h"
#include "SocketData.h"
#include "NetworkInterface.h"

#include "ThreadManager.h"


Session::Session() : NetworkObject()
{
	mRecvPkt = PacketRecvBuf(static_cast<INT32>(PacketRecvBuf::Info::Size));
	
	SocketData sockdata = {};
	sockdata.CreateSocket();
	NetworkObject::SetSocketData(sockdata);


}

Session::~Session()
{
}

void Session::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	OverlappedIO::Type IoType = overlapped->GetIoType();
	int ThreadID = TLS_MGR->Get_TlsInfoData()->id;

	switch (IoType)
	{

	case OverlappedIO::Type::Accept:
		std::cout << "Thread " << ThreadID << " Dispatch Accept\n";
		break;
	case OverlappedIO::Type::Connect:
		std::cout << "Thread " << ThreadID << " Dispatch Connect\n";
		break;
	case OverlappedIO::Type::DisConnect:
		std::cout << "Thread " << ThreadID << " Dispatch DisConnect\n";
		break;
	case OverlappedIO::Type::Send:
		std::cout << "Thread " << ThreadID << " Dispatch Send\n";
		break;
	case OverlappedIO::Type::Recv:
		std::cout << "Thread " << ThreadID << " Dispatch Recv : " << bytes << " bytes" << "\n";
		break;
	}

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

		/* Clear & Set - Connect Overlapped IO Object */
		mConnect.Clear_OVERLAPPED();
		mConnect.SetOwner(shared_from_this());

		/* Register Connect I/O */
		DWORD numOfBytes = {};
		bool ConnectExResult = NETWORK_MGR->ConnectEx()(GetSocketData().GetSocket()
										, reinterpret_cast<SOCKADDR*>(&GetSocketData().GetSockAddr())
										, sizeof(GetSocketData().GetSockAddr())
										, nullptr
										, 0
										, &numOfBytes
										, &mConnect);
		
		if (ConnectExResult == false) {
			INT32 errCode = ::WSAGetLastError();
			if (errCode != WSA_IO_PENDING) {
				mConnect.SetOwner(nullptr);
			}
		}
	}
		break;
		/// +-------------------
		///	REGISTER DISCONNECT
		/// -------------------+
	case OverlappedIO::Type::DisConnect:
	{
		mDisconnect.Clear_OVERLAPPED();
		mDisconnect.SetOwner(shared_from_this());

		bool DisconnectExResult = NETWORK_MGR->DiscconectEx()(GetSocketData().GetSocket()
															, &mDisconnect
															, TF_REUSE_SOCKET /* 소켓이 종료되어도 해당 주소와 포트를 다른 소켓이 사용가능 */
															, 0);

		if (DisconnectExResult == false) {
			INT32 errCode = ::WSAGetLastError();
			if (errCode != WSA_IO_PENDING) {
				mDisconnect.SetOwner(nullptr);
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

		mSend.Clear_OVERLAPPED();
		mSend.SetOwner(shared_from_this());

		/* Push SendPkt to Overlapped_Send */
		{
			while (mSendPkt_Queue.empty() == false) {
				SPtr_SendPktBuf sendPktBuf = mSendPkt_Queue.front();
				mSendPkt_Queue.pop();
				if(sendPktBuf)
					mSend.BufPush(sendPktBuf);

			}
		}

		/* Scatter-Gather */
		std::vector<WSABUF> wsaBufs = { };
		wsaBufs.reserve(mSend.BufSize());
		for (SPtr_SendPktBuf sendBuf : mSend.GetSendBuffers()) {
		
			WSABUF wsaBuf = {};
			wsaBuf.buf    = reinterpret_cast<char*>(sendBuf->GetBuffer());
			wsaBuf.len    = static_cast<LONG>(sendBuf->GetWriteSize());
			wsaBufs.push_back(wsaBuf);
		}

		/* ::WSASend */
		DWORD numOfBytes = 0;
		if (SOCKET_ERROR == ::WSASend(GetSocketData().GetSocket()
									, wsaBufs.data()
									, static_cast<DWORD>(wsaBufs.size())
									, OUT & numOfBytes
									, 0
									, &mSend
									, nullptr))
		{
			INT32 errCode = ::WSAGetLastError();
			if (errCode != WSA_IO_PENDING)
			{
				ProcessError(errCode);
				mSend.SetOwner(nullptr); // shared_ptr Release
				mSend.BufClear();
				mIsSendRegistered.store(false);
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
		mRecv.Clear_OVERLAPPED();
		mRecv.SetOwner(shared_from_this());

		/* ::WSARecv */
		WSABUF wsaBuf{};
		wsaBuf.buf = reinterpret_cast<char*>(mRecvPkt.GetWritePos());
		wsaBuf.len = mRecvPkt.GetFreeSize();

		DWORD numOfBytes = 0;
		DWORD flags      = 0;
		int WSARecvResult = ::WSARecv(GetSocketData().GetSocket()
									, &wsaBuf
									, 1
									, &numOfBytes
									, &flags
									, &mRecv
									, nullptr);

		if (WSARecvResult == SOCKET_ERROR) {
			INT32 errCode = ::WSAGetLastError();
			if (errCode != WSA_IO_PENDING) {
				ProcessError(errCode);
				mRecv.SetOwner(nullptr);
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
		mConnect.SetOwner(nullptr); // Shared_ptr -> release
		mIsConnected.store(true);

		GetOwnerNI()->AddSession(std::static_pointer_cast<Session>(shared_from_this()));

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
		mDisconnect.SetOwner(nullptr); // Shared_ptr -> release 

		OnDisconnected();
		GetOwnerNI()->ReleaseSession(std::static_pointer_cast<Session>(shared_from_this()));



	}
	break;
	/// +-------------------
	///    PROCESS SEND
	/// -------------------+
	case OverlappedIO::Type::Send:
	{
		mSend.SetOwner(nullptr); // shared_ptr -> release 
		

		/* Exception Disconnect */
		if (BytesTransferred == 0) {
			Disconnect(L"Send 0 - Disconnect!");
			break;
		}

		OnSend(BytesTransferred);

		if (mSendPkt_Queue.empty()) {
			mIsSendRegistered.store(false);
		}
		else {
			RegisterIO(OverlappedIO::Type::Send);
		}

	}
	break;
	/// +-------------------
	///    PROCESS RECV
	/// -------------------+
	case OverlappedIO::Type::Recv:
	{
		mRecv.SetOwner(nullptr); // shared_ptr -> Release 

		/* Exception Disconnect */
		if (BytesTransferred == 0) {
			Disconnect(L"Recv 0 - Disconnect!");
			break;
		}
		if (FALSE == mRecvPkt.OnWrite(BytesTransferred)) {
			Disconnect(L"OnWirte Overflow - Disconnect!");
			break;
		}

		/* On Recv */
		UINT32 DataSize = mRecvPkt.GetDataSize();
		UINT32 ProcessLen = OnRecv(mRecvPkt.GetReadPos(), DataSize);
		if (FALSE == (ProcessLen < 0 || DataSize < ProcessLen || mRecvPkt.OnRead(ProcessLen))) {
			Disconnect(L"OnRead Overflow - Disconnect!");
			break;
		}

		/* Register IO */
		mRecvPkt.Clean();
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

	volatile bool RegisterSend = false;

	{

		mSendPkt_Queue.push(buf);

		if (mIsSendRegistered.exchange(true) == false)
			RegisterSend = true;
	}

	if (RegisterSend)
		RegisterIO(OverlappedIO::Type::Send);

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


