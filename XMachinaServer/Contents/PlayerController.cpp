#include "pch.h"
#include "PlayerController.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "ServerLib/ThreadManager.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}


bool PlayerController::EnterPlayer(SPtr_GamePlayer player)
{
	mCurrPlayerCnt.fetch_add(1);

	{
		mSRWLock.LockWrite();
		mGamePlayers[player->GetID()] = player;
		player->setRoomID(mRoomID);

		mSRWLock.UnlockWrite();
	}


	LOG_MGR->SetColor(TextColor::BrightCyan);
	LOG_MGR->Cout(mRoomID, " - ROOM - ", player->GetID(), " ENTER SUCCESS\n");
	LOG_MGR->SetColor(TextColor::Default);


	return true;
}

void PlayerController::Init(int roomID, SPtr_GameRoom owner)
{
	mRoomID    = roomID;
	mOwnerRoom = owner;

}

bool PlayerController::ExitPlayer(UINT32 sessionID)
{
	mCurrPlayerCnt.fetch_sub(1);

	{
		mSRWLock.LockWrite();
		mGamePlayers.unsafe_erase(sessionID);
		mSRWLock.UnlockWrite();
	}

	LOG_MGR->SetColor(TextColor::BrightMagenta);
	LOG_MGR->Cout(mRoomID, " - ROOM ", sessionID, " EXIT SUCCESS\n");
	LOG_MGR->SetColor(TextColor::Default);


	return true;
}

SPtr_GamePlayer PlayerController::FindPlayer(UINT32 sessionID)
{
	//mSRWLock.LockRead();

	size_t size = mGamePlayers.size();
	auto obj = mGamePlayers.find(sessionID);
	if (obj == mGamePlayers.end())
	{
		//mSRWLock.UnlockRead();
		return nullptr;
	}

	//mSRWLock.UnlockRead();
	return obj->second;
}

void PlayerController::Broadcast(SPtr_SendPktBuf spkt, UINT32 exceptSessionID)
{
	mSRWLock.LockRead();

	for (auto& player : mGamePlayers) {
		if (player.first == exceptSessionID) continue;
		SPtr_GameSession session = player.second->GetInfo().Owner;
		session->Send(spkt);

	}

	mSRWLock.UnlockRead();
}

void PlayerController::SendPacket(UINT32 sessionID, SPtr_SendPktBuf sendPkt)
{
	mSRWLock.LockRead();

	const auto& iter = mGamePlayers.find(sessionID);
	if (iter != mGamePlayers.end()) {
		SPtr_GameSession session = iter->second->GetInfo().Owner;
		if (sendPkt != nullptr)
			session->Send(sendPkt);
	}

	mSRWLock.UnlockRead();
}

std::vector<PlayerInfo> PlayerController::GetInsertedPlayersInfo()
{
	mSRWLock.LockRead();

	std::vector<PlayerInfo> playerInfos = {};
	for (auto& player : mGamePlayers) {
		playerInfos.push_back(player.second->GetInfo());
	}

	mSRWLock.UnlockRead();

	return playerInfos;
}

