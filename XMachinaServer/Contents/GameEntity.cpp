#include "pch.h"
#include "GameEntity.h"

GameEntity::GameEntity()
{
}

GameEntity::GameEntity(const GameEntity& other)
{
}

GameEntity::GameEntity(int id)
	: NetworkObject(id)
{
}

GameEntity::~GameEntity()
{
}

void GameEntity::Activate()
{
	mIsActive = true;
}

void GameEntity::DeActivate()
{
	mIsActive = false;
}
