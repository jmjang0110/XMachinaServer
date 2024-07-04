#include "pch.h"
#include "GameBullet.h"

GameBullet::GameBullet()
	: GameObject()
{
}

GameBullet::GameBullet(UINT32 sessionID)
	: GameObject(sessionID)
{
}

GameBullet::GameBullet(UINT32 sessionID, SPtr_GameSession owner)
	: GameObject(sessionID)
{
}

GameBullet::~GameBullet()
{
}

void GameBullet::Update()
{
}

void GameBullet::WakeUp()
{
}

void GameBullet::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}
