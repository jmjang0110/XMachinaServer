#include "pch.h"
#include "GameMonster.h"

GameMonster::GameMonster()
	: GameObject(-1)
{
}

GameMonster::GameMonster(UINT32 id)
	: GameObject(id)
{
	
}

GameMonster::~GameMonster()
{
}

void GameMonster::Update()
{
}

void GameMonster::WakeUp()
{
}

void GameMonster::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{


}


