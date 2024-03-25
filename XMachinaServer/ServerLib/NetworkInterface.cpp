#include "pch.h"
#include "NetworkInterface.h"
#include "NetworkManager.h"

#include "ServerNetwork.h"
#include "../Framework.h"


NetworkInterface::~NetworkInterface()
{
	if (mIocpHandle) {
		::CloseHandle(mIocpHandle);
	}
}

bool NetworkInterface::RegisterIocp(SPtr_NetObj netObj)
{
	// Iocp Handle �� ���ؼ� �ش� ������ �����ϵ��� ����Ѵ�.
	SOCKET sock = netObj->GetSocketData().GetSocket();
	HANDLE handle = netObj->GetSocketHandle();
	if (handle) {
		return ::CreateIoCompletionPort(handle
									, mIocpHandle
									, 0
									, 0);
	}
	return false;
}

bool NetworkInterface::Dispatch_CompletedTasks_FromIOCP(UINT32 msTimeOut)
{
	/// +=========================== IOCP Queue ============================
	///  (�Ϸ�� �ϰ���...)
	/// ____________________________________________________________________
	/// (+_+) �����Ѵ�...
	/// (timeoutMs : INFINITE -> ���Ѵ�� (���ŷ) / 0 -> (����ŷ)
	/// +==================================================================+

	DWORD				BytesTransferred = 0;
	ULONG_PTR			key              = 0;
	OverlappedObject*	overObj          = nullptr;

	LPOVERLAPPED lpOverlapped = nullptr;

	auto listener = FRAMEWORK->GetServerNetwork()->GetListener();

	bool IsTaskExisted = ::GetQueuedCompletionStatus(mIocpHandle
												, OUT &BytesTransferred
												, OUT &key
												, OUT &lpOverlapped/*reinterpret_cast<LPOVERLAPPED*>(&overObj)*/
												, msTimeOut);
	if (TRUE == IsTaskExisted) {
		/// +------------
		///    SUCCESS 
		/// ------------+
		overObj = reinterpret_cast<OverlappedObject*>(lpOverlapped);
		SPtr_NetObj netObj = overObj->GetOwner();
		netObj->Dispatch(overObj, BytesTransferred);

	}
	else
	{
		overObj = reinterpret_cast<OverlappedObject*>(lpOverlapped);

		/// +------------
		///     FAIL
		/// ------------+
		INT32 errCode = ::WSAGetLastError();
		// TODO : Log
		//std::cout << "Handle Error : " << errCode << std::endl;

		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			/* errCode : 64 = Disconnect ( Recv 0 ) */
			SPtr_NetObj netObj = overObj->GetOwner();
			netObj->Dispatch(overObj, BytesTransferred);
			break;
		}

	}


	return true;
}

bool NetworkInterface::Start(std::wstring ip, UINT16 portNum)
{
	/* Sock Addr */
	::memset(&mSockAddr, 0, sizeof(mSockAddr));
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_addr   = NETWORK_MGR->Ip2Address(ip.c_str());
	mSockAddr.sin_port   = ::htons(portNum); // ��Ʈ ��ȣ : 16��Ʈ �����̴�. host to Network - short(16bit) ( htons )

	/* IO Completion Port - Handle */
	mIocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE
											, 0
											, 0
											, 0);

	return mIocpHandle != INVALID_HANDLE_VALUE;
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
	if(RegisterIocp(session) == false)
		return nullptr;
	return session;
}

void NetworkInterface::AddSession(SPtr_Session session)
{
}

void NetworkInterface::ReleaseSession(SPtr_Session session)
{
}
