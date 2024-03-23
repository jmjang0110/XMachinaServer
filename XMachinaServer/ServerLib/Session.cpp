#include "pch.h"
#include "Session.h"
#include "PacketSendBuf.h"
#include "NetworkManager.h"
#include "SocketData.h"



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

		/* PacketRecvBuf 등록 */
		WSABUF wsaBuf{};
		wsaBuf.buf = nullptr; //reinterpret_cast<CHAR*>();
		wsaBuf.len = 0;

		DWORD numOfBytes = {};
		DWORD flags      = {};
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

void Session::ProcessIO(OverlappedIO::Type IoType)
{
	switch (IoType)
	{
		/// +-------------------
		///    PROCESS CONNECT
		/// -------------------+
	case OverlappedIO::Type::Connect:
	{
		mConnect.SetOwner(nullptr);
		mIsConnected.store(true);


	}
		break;
		/// +-------------------
		///  PROCESS DISCONNECT
		/// -------------------+
	case OverlappedIO::Type::DisConnect:
	{

	}
		break;
		/// +-------------------
		///    PROCESS SEND
		/// -------------------+
	case OverlappedIO::Type::Send:
	{

	}
		break;
		/// +-------------------
		///    PROCESS RECV
		/// -------------------+
	case OverlappedIO::Type::Recv:
	{

	}
		break;
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


