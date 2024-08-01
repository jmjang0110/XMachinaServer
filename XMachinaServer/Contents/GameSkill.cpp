#include "pch.h"
#include "GameSkill.h"

GameSkill::GameSkill() 
	: GameObject()

{
}

GameSkill::GameSkill(UINT32 sessionID) 
	: GameObject(sessionID)

{
}

GameSkill::GameSkill(UINT32 sessionID, SPtr<GamePlayer> owner) 
	: GameObject(sessionID)
{
	mOwnerPlayer = owner;

}

GameSkill::~GameSkill()
{
}

void GameSkill::Update()
{
}

void GameSkill::Activate()
{
	GameObject::Activate();

	mActivate_Ref.fetch_add(1);

	if (mActivate_Ref.load() == 1) {
		GameObject::RegisterUpdate();
	}
}

void GameSkill::DeActivate()
{
	GameObject::DeActivate();

	mActivate_Ref.fetch_sub(1);
}

void GameSkill::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}
