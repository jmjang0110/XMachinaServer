#include "pch.h"
#include "GameMonster.h"

GameMonster::GameMonster()
	: GameObject(-1)
{
}

GameMonster::GameMonster(UINT32 id, Coordinate sectorIdx)
	: GameObject(id)
{
	mSectorIndex = sectorIdx;
}

GameMonster::~GameMonster()
{
}

void GameMonster::Update()
{
	GameObject::Update();

}

void GameMonster::WakeUp()
{
	GameObject::WakeUp();

}

void GameMonster::Start()
{
	GameObject::Start();

}

void GameMonster::Activate()
{
	GameObject::Activate();

}

void GameMonster::DeActivate()
{
	GameObject::DeActivate();

}

void GameMonster::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{


}


