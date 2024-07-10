#include "pch.h"
#include "GameEntity.h"

GameEntity::GameEntity()
{
}

GameEntity::GameEntity(int id)
	: mID(id)
{
}

GameEntity::~GameEntity()
{
}

void GameEntity::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}

void GameEntity::Activate()
{
}

void GameEntity::DeActivate()
{
}
