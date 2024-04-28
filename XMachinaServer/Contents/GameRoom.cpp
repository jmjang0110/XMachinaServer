#include "pch.h"
#include "GameRoom.h"

bool GameRoom::InsertPlayer(SPtr_GameSession owner)
{
	return false;
}

bool GameRoom::RemovePlayer(UINT32 sessionID)
{
	return false;
}

SPtr_GamePlayer GameRoom::FindPlayer(UINT32 sessionID)
{
	return SPtr_GamePlayer();
}

void GameRoom::Broadcast(SPtr_SendPktBuf spkt, UINT32 exceptSessionID)
{
}

void GameRoom::SendPacket(UINT32 sessionID, SPtr_SendPktBuf sendPkt)
{
}

std::vector<PlayerInfo> GameRoom::GetInsertedPlayersInfo()
{
	return std::vector<PlayerInfo>();
}

