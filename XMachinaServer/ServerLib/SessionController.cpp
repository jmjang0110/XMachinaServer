#include "pch.h"
#include "SessionController.h"

SessionController::SessionController()
{
}

SessionController::~SessionController()
{
}

SPtr_Session SessionController::CreateSession(SPtr_NI netInterfaceOwner)
{

	SPtr_Session session = mSessionConstructorFunc();
	session->SetOwerNetworkInterface(netInterfaceOwner);
	return session;
}

void SessionController::AddSession(UINT32 sessionID, SPtr_Session session)
{
#ifdef USE_MY_MADE_LOCK
	//mSessionsSpLock.Lock();
	WRITE_LOCK;
#else 
	mSessionsMutex.lock();
#endif // USE_MY_MADE_LOCK

	//mSessionsMutex.lock();
	//mSessionRWLock.lockWrite();

	//WRITE_LOCK;
	mCurrSessionCnt.fetch_add(1);
	session->SetID(sessionID);
	mSessionsMap[sessionID] = session;

	//mSessionRWLock.unlockWrite();
	//mSessionsMutex.unlock();
#ifdef USE_MY_MADE_LOCK
	//mSessionsSpLock.UnLock();
#else 
	mSessionsMutex.unlock();
#endif // USE_MY_MADE_LOCK
}

void SessionController::ReleaseSession(UINT32 sessionID)
{
	//mSessionRWLock.lockWrite();
	//mSessionsMutex.lock();

#ifdef USE_MY_MADE_LOCK
	//mSessionsSpLock.Lock();
	WRITE_LOCK;
#else 
	mSessionsMutex.lock();
#endif // USE_MY_MADE_LOCK

	mCurrSessionCnt.fetch_sub(1);

	auto iter = mSessionsMap.find(sessionID);
	if (iter != mSessionsMap.end()) {
		mSessionsMap.unsafe_erase(iter);
		std::cout << "Session with key " << sessionID << " removed from the map." << std::endl;
	}
	else {
		std::cout << "Session with key " << sessionID << " not found in the map." << std::endl;
	}

#ifdef USE_MY_MADE_LOCK
	//mSessionsSpLock.UnLock();
	
#else 
	mSessionsMutex.unlock();
#endif // USE_MY_MADE_LOCK	//mSessionsMutex.unlock();
	//mSessionRWLock.unlockWrite();
}

void SessionController::Broadcast(SPtr_SendPktBuf sendBuf)
{
	//Lock::RWLock::GetInst()->lockWrite();		
	//mSessionRWLock.lockWrite();

	//mSessionsMutex.lock();
#ifdef USE_MY_MADE_LOCK
	//mSessionsSpLock.Lock();
	WRITE_LOCK;
#else 
	mSessionsMutex.lock();
#endif // USE_MY_MADE_LOCK

	//WRITE_LOCK;

	for (const auto& iter : mSessionsMap) {
		SPtr_Session session = iter.second;
		if (session) {
			std::cout << session.get() << std::endl;
			iter.second->Send(sendBuf);
		}
	
	}

	//mSessionRWLock.unlockWrite();
	//Lock::RWLock::GetInst()->unlockWrite();
	//mSessionsSpLock.UnLock();
#ifdef USE_MY_MADE_LOCK
	//mSessionsSpLock.UnLock();
#else 
	mSessionsMutex.unlock();
#endif // USE_MY_MADE_LOCK

	//mSessionsMutex.unlock();
}

void SessionController::Send(UINT32 sessionID, SPtr_SendPktBuf sendBuf)
{

}
