#include "pch.h"
#include "Listener.h"
#include "SocketData.h"
#include "NetworkInterface.h"
#include "Session.h"
#include "NetworkManager.h"




Listener::~Listener()
{
	Close();
	for (auto obj : mAccepts) {
		SAFE_DELETE(obj);
	}

}


void Listener::Register_OverlappedIO_Accept(Overlapped_Accept* overlapped)
{
	SPtr_Session session = mOwnerNI->CreateSession(); // Register IOCP 

	overlapped->Clear_OVERLAPPED();
	overlapped->SetSession(session);

	DWORD recvBytes     = 0;

	SOCKET ListenSock  = mListenSocket.GetSocket();
	SOCKET ClientSock  = session->GetSocketData().GetSocket();
	BYTE*  RecvBufPtr  = session->GetRecvPktBuf().GetWritePos();

	bool result       = NETWORK_MGR->AcceptEx()(ListenSock 
											  , ClientSock 
											  , RecvBufPtr 
											  , 0
											  , sizeof(SOCKADDR_IN) + 16
											  , sizeof(SOCKADDR_IN) + 16
											  , OUT &recvBytes
											  , static_cast<LPOVERLAPPED>(overlapped));
											  
	if (FALSE == result) {
		const INT32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			// Re Register Overlapped IO Accept 
			Register_OverlappedIO_Accept(overlapped);
		}
	}
	
}

void Listener::Process_OverlappedIO_Accept(Overlapped_Accept* overlapped)
{
	SPtr_Session session = overlapped->GetSession();

	if (false == session->GetSocketData().SetUpdateAcceptSocket(mListenSocket.GetSocket())){
		Register_OverlappedIO_Accept(overlapped);
		return;
	}

	SOCKADDR_IN sockAddress;
	INT32 sizeofSockAddr = sizeof(sockAddress);
	if(SOCKET_ERROR == ::getpeername(session->GetSocketData().GetSocket()
									, OUT reinterpret_cast<SOCKADDR*>(&sockAddress)
									, &sizeofSockAddr))
	{
		Register_OverlappedIO_Accept(overlapped);
		return;
	}

	/// +--------------------
	///   Client Connected
	/// --------------------+
	session->GetSocketData().SetSockAddrIn(sockAddress);
	session->ProcessIO(OverlappedIO::Type::Connect);
	
	Register_OverlappedIO_Accept(overlapped); // ´Ù½Ã Accept

}

void Listener::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	Overlapped_Accept* accept = reinterpret_cast<Overlapped_Accept*>(overlapped);
	Process_OverlappedIO_Accept(accept);
}


bool Listener::Start(SPtr_NI netInterface)
{
	mOwnerNI = netInterface;

	if (mListenSocket.CreateSocket() == INVALID_SOCKET)
		return false;

	mListenSocket.SetSockAddrIn(netInterface->GetSockAddr());
	
	/* Register Iocp */
	if (mOwnerNI->RegisterIocp(shared_from_this()) == false)		return false;
	
	/* Create Listen Socket + Update Socket Option */
	if (mListenSocket.SetReuseAddress(true) == false)				return false;
	if (mListenSocket.SetLinger(0, 0) == false)						return false;
	if (mListenSocket.Bind(mListenSocket.GetSockAddr()) == false)	return false;
	if (mListenSocket.Listen() == false)							return false;

	/* Register Accept IO */
	const UINT32 MaxAcceptCnt = mOwnerNI->GetMaxSEssionCnt();
	for (INT32 i = 0; i < MaxAcceptCnt; ++i) {
		Overlapped_Accept* acceptIO = new Overlapped_Accept;
		acceptIO->SetOwner(shared_from_this());
		mAccepts.push_back(acceptIO);
		Register_OverlappedIO_Accept(acceptIO);
	}

	return true;
}


void Listener::Close()
{
	mListenSocket.Close();

}

