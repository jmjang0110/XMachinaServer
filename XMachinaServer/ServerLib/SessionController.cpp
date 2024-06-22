#include "pch.h"
#include "SessionController.h"
#include "ThreadManager.h"

SessionController::SessionController()
{
}

SessionController::~SessionController()
{
	for (int i = 0; i < mSessionsMap.size(); ++i) {
		mSessionsMap[i] = nullptr;
	}
	mSessionsMap.clear();
}

SPtr_Session SessionController::CreateSession(SPtr_NI netInterfaceOwner)
{

	SPtr_Session session = mSessionConstructorFunc();
	session->SetOwerNetworkInterface(netInterfaceOwner);
	return session;
}

void SessionController::AddSession(UINT32 sessionID, SPtr_Session session)
{
	mSRWLock.LockWrite();

	if (mCurrSessionCnt < static_cast<int>(mMaxSessionCnt)) {
		mCurrSessionCnt.fetch_add(1);
		mSessionsMap[sessionID] = session;
	}


	mSRWLock.UnlockWrite();
}

void SessionController::ReleaseSession(UINT32 sessionID)
{
	mSRWLock.LockWrite();

	auto iter = mSessionsMap.find(sessionID);
	if (iter != mSessionsMap.end()) {
		mSessionsMap.unsafe_erase(iter);
		LOG_MGR->SetColor(TextColor::BrightRed);
		LOG_MGR->Cout("Session with key ", sessionID, " removed from the map.\n");
		LOG_MGR->SetColor(TextColor::Default);

	}
	else {
		LOG_MGR->SetColor(TextColor::Magenta);
		LOG_MGR->Cout("Session with key ", sessionID, " not found in the map.\n");
		LOG_MGR->SetColor(TextColor::Default);

	}

	mSRWLock.UnlockWrite();
}

void SessionController::Broadcast(SPtr_SendPktBuf sendBuf)
{
	//LOG_MGR->Cout("[", TLS_MGR->Get_TlsInfoData()->id, "] - ", "LOCKREAD 시도\n");
	mSRWLock.LockWrite();
	//LOG_MGR->Cout("[", TLS_MGR->Get_TlsInfoData()->id, "] - ", "LOCKREAD 성공");

	for (const auto& iter : mSessionsMap) {
		SPtr_Session session = iter.second;
		iter.second->Send(sendBuf);
	}

	//LOG_MGR->Cout("[", TLS_MGR->Get_TlsInfoData()->id, "] - ", "UNLOCK READ 시도\n");
	mSRWLock.UnlockWrite();
	//LOG_MGR->Cout("[", TLS_MGR->Get_TlsInfoData()->id, "] - ", "UNLOCK READ 성공\n");
}

void SessionController::Send(UINT32 sessionID, SPtr_SendPktBuf sendBuf)
{
	LOG_MGR->Cout("[", TLS_MGR->Get_TlsInfoData()->id, "] - ", "LOCKREAD 시도");
	mSRWLock.LockRead();
	LOG_MGR->Cout("[", TLS_MGR->Get_TlsInfoData()->id, "] - ", "LOCKREAD 성공");

	auto se = mSessionsMap.find(sessionID);
	if (se != mSessionsMap.end()) {
		se->second->Send(sendBuf);
	}

	LOG_MGR->Cout("[", TLS_MGR->Get_TlsInfoData()->id, "] - ", "UNLOCK READ 시도");
	mSRWLock.UnlockRead();
	LOG_MGR->Cout("[", TLS_MGR->Get_TlsInfoData()->id, "] - ", "UNLOCK READ 성공");


}
