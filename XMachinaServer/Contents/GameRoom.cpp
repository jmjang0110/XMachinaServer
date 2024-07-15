#include "pch.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "ServerLib/ThreadManager.h"
#include "PlayerController.h"
#include "NPCController.h"
#include "SectorController.h"
#include "GameOverlapped.h"
#include "Framework.h"
#include "ServerLib/ServerNetwork.h"
#include "ServerLib/MemoryManager.h"


GameRoom::GameRoom()
{
}


GameRoom::~GameRoom()
{
	SAFE_DELETE(mPC);
	SAFE_DELETE(mNC);
	SAFE_DELETE(mSC);

}

void GameRoom::PQCS(OverlappedObject* over)
{
	::PostQueuedCompletionStatus(SERVER_NETWORK->GetIocpHandle(), 1, 0, over);

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

	mPC->Init(roomid, std::static_pointer_cast<GameRoom>(shared_from_this()));
	mSC->Init(std::static_pointer_cast<GameRoom>(shared_from_this()));
	mNC->Init(std::static_pointer_cast<GameRoom>(shared_from_this()));


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

std::vector<PlayerSnapShot> GameRoom::GetInsertedPlayersInfo()
{
	return mPC->GetInsertedPlayersInfo();
}


/// +-------------------------------------------------------------------
///	¢º¢º¢º NPC Controller 
/// -------------------------------------------------------------------+
 


/// +-------------------------------------------------------------------
///	¢º¢º¢º Sector Controller 
/// -------------------------------------------------------------------+