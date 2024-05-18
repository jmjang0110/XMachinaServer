#include "pch.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "ServerLib/ThreadManager.h"

GameRoom::GameRoom()
{
}


GameRoom::~GameRoom()
{
}

bool GameRoom::EnterPlayer(SPtr_GamePlayer player)
{
	
	mCurrPlayerCnt.fetch_add(1);

	{
		mSRWLock.LockWrite();
		mGamePlayers[player->GetID()] = player;
		player->setRoomID(mID);
		mSRWLock.UnlockWrite();
	}


	LOG_MGR->SetColor(TextColor::BrightCyan);
	LOG_MGR->Cout(mID, " - ROOM - ", player->GetID(), " ENTER SUCCESS\n");
	LOG_MGR->SetColor(TextColor::Default);


	return true;
}

bool GameRoom::ExitPlayer(UINT32 sessionID)
{
	mCurrPlayerCnt.fetch_sub(1);

	{
		mSRWLock.LockWrite();
		mGamePlayers.unsafe_erase(sessionID);
		mSRWLock.UnlockWrite();
	}

	LOG_MGR->SetColor(TextColor::BrightMagenta);
	LOG_MGR->Cout(mID, " - ROOM ", sessionID, " EXIT SUCCESS\n");
	LOG_MGR->SetColor(TextColor::Default);


	return true;
}



SPtr_GamePlayer GameRoom::FindPlayer(UINT32 sessionID)
{
	//mSRWLock.LockRead();

	size_t size = mGamePlayers.size();
	auto obj = mGamePlayers.find(sessionID);
	if (obj == mGamePlayers.end())
	{
		mSRWLock.UnlockRead();
		return nullptr;
	}

	//mSRWLock.UnlockRead();
	return obj->second;
}


void GameRoom::Broadcast(SPtr_SendPktBuf spkt, UINT32 exceptSessionID)
{
	//mSRWLock.LockWrite();

	for (auto& player : mGamePlayers) {
		if (player.first == exceptSessionID) continue;
		SPtr_GameSession session = player.second->GetInfo().Owner;
		session->Send(spkt);

	}

	//mSRWLock.UnlockWrite();

}

void GameRoom::SendPacket(UINT32 sessionID, SPtr_SendPktBuf sendPkt)
{

}

std::vector<PlayerInfo> GameRoom::GetInsertedPlayersInfo()
{
	mSRWLock.LockWrite();

	std::vector<PlayerInfo> playerInfos = {};
	for (auto& player : mGamePlayers) {
		playerInfos.push_back(player.second->GetInfo());
	}

	mSRWLock.UnlockWrite();

	return playerInfos;
}

bool GameRoom::CheckIn()
{
	if (mCurrPlayerCnt.load() < RoomInfo::MaxSessionSize)
		return true;

	return false;
}

