#include "pch.h"
#include "GameWorld.h"
#include "GameOverlapped.h"
#include "Framework.h"
#include "ServerLib/ServerNetwork.h"
#include "ServerLib/MemoryManager.h"


DEFINE_SINGLETON(GameWorld);

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
	for (int i = 0; i < 100; ++i) {
		m_Mon_Ursacetus[i] = MEMORY->Make_Shared<Monster_Ursacetus>();
	}
	
}

void GameWorld::PQCS(OverlappedObject* over)
{
	::PostQueuedCompletionStatus(SERVER_NETWORK->GetIocpHandle(), 1, 0, over);
}
