#include "pch.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "ServerLib/ThreadManager.h"

GameRoom::GameRoom()
{
}


GameRoom::~GameRoom()
{
	SAFE_DELETE(mPC);
	SAFE_DELETE(mNC);
	SAFE_DELETE(mSC);

}

/// +-------------------------------------------------------------------
///	¢º¢º¢º Room
/// -------------------------------------------------------------------+
void GameRoom::Init(int roomid)
{
	SetRoomID(roomid);

	mPC = new PlayerController;
	mNC = new NPCController;
	mSC = new SectorController;

	mPC->Init(roomid);

}

bool GameRoom::IsPossibleToEnter()
{
	if (mPC->GetPlayersSize() < RoomInfo::MaxSessionSize)
		return true;

	return false;
}


/// +-------------------------------------------------------------------
///	¢º¢º¢º Player Controller 
/// -------------------------------------------------------------------+
bool GameRoom::EnterPlayer(SPtr_GamePlayer player)
{
	return mPC->EnterPlayer(player);
}

bool GameRoom::ExitPlayer(UINT32 id)
{
	return mPC->ExitPlayer(id);
}

void GameRoom::Broadcast(SPtr_SendPktBuf packet, UINT32 exceptsessionid)
{
	mPC->Broadcast(packet, exceptsessionid);
}

void GameRoom::SendPacket(UINT32 sessionid, SPtr_SendPktBuf packet)
{
	mPC->SendPacket(sessionid, packet);
}

std::vector<PlayerInfo> GameRoom::GetInsertedPlayersInfo()
{
	return mPC->GetInsertedPlayersInfo();
}


/// +-------------------------------------------------------------------
///	¢º¢º¢º NPC Controller 
/// -------------------------------------------------------------------+
 


/// +-------------------------------------------------------------------
///	¢º¢º¢º Sector Controller 
/// -------------------------------------------------------------------+