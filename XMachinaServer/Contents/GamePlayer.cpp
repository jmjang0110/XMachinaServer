#include "pch.h"
#include "GamePlayer.h"
#include "PlayerController.h"
#include "Sector.h"
#include "SectorController.h"
#include "NPCController.h"

#include "GameRoom.h"

GamePlayer::GamePlayer()
	: GameObject(-1)
{
}

GamePlayer::GamePlayer(UINT32 sessionID, SPtr_GameSession owner)
	: GameObject(sessionID)
{

	GameObject::SetType(GameObjectInfo::Type::GamePlayer);

	mInfo.PlayerID = sessionID;
	mInfo.Owner    = owner;
}

GamePlayer::~GamePlayer()
{
	mInfo.Owner = nullptr;
}

void GamePlayer::Update()
{
	GameObject::Update();

	/* Update View List */
	mInfo.Vlist = mOwnerPC->GetOwnerRoom()->GetSectorController()->GetViewList(mInfo.Position, mInfo.ViewRangeRadius);
	
}

void GamePlayer::WakeUp()
{
	GameObject::WakeUp();

}

void GamePlayer::Start()
{
	GameObject::Start();

}

void GamePlayer::Activate()
{
	GameObject::Activate();

}

void GamePlayer::DeActivate()
{
	GameObject::DeActivate();

}

void GamePlayer::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}
