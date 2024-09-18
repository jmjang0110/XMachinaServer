#include "pch.h"
#include "GameRoom.h"
#include "GameObject.h"
#include "GameSession.h"

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

void GameRoom::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}

/// +-------------------------------------------------------------------
///	¢º¢º¢º Room
/// -------------------------------------------------------------------+
void GameRoom::Init(int roomid)
{
	SetID(roomid);
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
bool GameRoom::EnterPlayer(SPtr<GameObject> player)
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


std::vector<SPtr<GameObject>> GameRoom::GetallPlayers()
{
	return mPC->GetAllPlayers();
}

bool GameRoom::CollideCheckWithNPC(SPtr<GameObject> obj, ObjectTag objTag)
{
	float CheckSectorRadius = 10.f;
	bool isCollide = false;
	if (ObjectTag::Building == objTag) {
		std::vector<Coordinate> checkSectors = mSC->GetCheckSectors(obj->GetTransform()->GetSnapShot().GetPosition(), CheckSectorRadius);
		for (int i = 0; i < checkSectors.size(); ++i) {
			isCollide = mSC->CollideCheck_WithBuildings(checkSectors[i], obj);
			if (isCollide)
				return isCollide;
		}
	}
	else if (ObjectTag::Enemy == objTag) {
		std::vector<Coordinate> checkSectors = mSC->GetCheckSectors(obj->GetTransform()->GetSnapShot().GetPosition(), CheckSectorRadius);
		for (int i = 0; i < checkSectors.size(); ++i) {
			isCollide = mSC->CollideCheck_WithEnemies(checkSectors[i], obj);
			if (isCollide)
				return isCollide;
		}
	}
	return isCollide;
}


/// +-------------------------------------------------------------------
///	¢º¢º¢º NPC Controller 
/// -------------------------------------------------------------------+
 


/// +-------------------------------------------------------------------
///	¢º¢º¢º Sector Controller 
/// -------------------------------------------------------------------+