#include "pch.h"
#include "SessionController.h"

SessionController::SessionController()
{
}

SessionController::~SessionController()
{
	for (int i = 0; i < mSessionsMap.size(); ++i) {
		mSessionsMap[i] = nullptr;
	}

}

SPtr_Session SessionController::CreateSession(SPtr_NI netInterfaceOwner)
{

	SPtr_Session session = mSessionConstructorFunc();
	session->SetOwerNetworkInterface(netInterfaceOwner);
	return session;
}

void SessionController::AddSession(UINT32 sessionID, SPtr_Session session)
{

	//mSessionsMutex.lock();
	//mSessionRWLock.lockWrite();

	WRITE_LOCK;
	mCurrSessionCnt.fetch_add(1);
	mSessionsMap[sessionID] = session;

	//mSessionRWLock.unlockWrite();
	//mSessionsMutex.unlock();

}

void SessionController::ReleaseSession(UINT32 sessionID)
{
	//mSessionRWLock.lockWrite();
	WRITE_LOCK;

	auto iter = mSessionsMap.find(sessionID);
	if (iter != mSessionsMap.end()) {
		mSessionsMap.unsafe_erase(iter);
		std::cout << "Session with key " << sessionID << " removed from the map." << std::endl;
	}
	else {
		std::cout << "Session with key " << sessionID << " not found in the map." << std::endl;
	}

	//mSessionRWLock.unlockWrite();
}

void SessionController::Broadcast(SPtr_SendPktBuf sendBuf)
{
	//Lock::RWLock::GetInst()->lockWrite();		
	//mSessionRWLock.lockWrite();

	//mSessionsMutex.lock();

	WRITE_LOCK;

	for (const auto& iter : mSessionsMap) {
		SPtr_Session session = iter.second;
		iter.second->Send(sendBuf);
	}

	//mSessionRWLock.unlockWrite();
	//Lock::RWLock::GetInst()->unlockWrite();

	//mSessionsMutex.unlock();
}

void SessionController::Send(UINT32 sessionID, SPtr_SendPktBuf sendBuf)
{

}
