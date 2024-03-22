#include "pch.h"
#include "NetworkInterface.h"
#include "NetworkManager.h"


NetworkInterface::NetworkInterface(std::wstring ip, UINT16 portNum)
{
	/* Sock Addr */
	::memset(&mSockAddr, 0, sizeof(mSockAddr));
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_addr   = NETWORK_MGR->Ip2Address(ip.c_str());
	mSockAddr.sin_port   = ::htons(portNum); // 포트 번호 : 16비트 숫자이다. host to Network - short(16bit) ( htons )

	/* IO Completion Port - Handle */
	mIocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE
											, 0
											, 0
											, 0);

	assert(mIocpHandle != INVALID_HANDLE_VALUE);

}

NetworkInterface::~NetworkInterface()
{
	::CloseHandle(mIocpHandle);
}

bool NetworkInterface::RegisterIocp(SPtr_NetObj netObj)
{
	// Iocp Handle 을 통해서 해당 소켓을 관찰하도록 등록한다.
	return ::CreateIoCompletionPort(netObj->GetSocketHandle()
									, mIocpHandle
									, 0
									, 0);
}

void NetworkInterface::Close()
{
}

void NetworkInterface::BroadCast(SPtr_SendPktBuf sendBuf)
{
}

void NetworkInterface::Send(UINT32 sessionID, SPtr_SendPktBuf sendBuf)
{
}


SPtr_Session NetworkInterface::CreateSession()
{
	SPtr_Session session = mSessionConstructorFunc();
	session->SetOwerNetworkInterface(shared_from_this());
	RegisterIocp(session);
	return session;
}

void NetworkInterface::AddSession(SPtr_Session session)
{
}

void NetworkInterface::ReleaseSession(SPtr_Session session)
{
}
