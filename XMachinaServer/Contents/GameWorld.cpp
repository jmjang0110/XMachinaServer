#include "pch.h"
#include "GameWorld.h"
#include "GameOverlapped.h"
#include "Framework.h"
#include "ServerLib/ServerNetwork.h"
#include "ServerLib/MemoryManager.h"

#include "Script_Ursacetus.h"
#include "Script_Phero.h"
#include "Script_PheroDropper.h"
#include "Script_BehaviorTree.h"
#include "Script_DefaultEnemyBT.h"

#include "Transform.h"

DEFINE_SINGLETON(GameWorld);

//GameWorld::GameWorld()
//{
//}
//
//GameWorld::~GameWorld()
//{
//
//}
//
//void GameWorld::Init()
//{
//	for (int i = 0; i < 1; ++i) {
//		m_Mon_Ursacetus[i] = std::make_shared<Monster_Ursacetus>(i, Coordinate(0,0));
//
//		m_Mon_Ursacetus[i]->AddComponent<Transform>(ComponentInfo::Type::Transform);
//
//		m_Mon_Ursacetus[i]->AddScript<Script_Ursacetus>(ScriptInfo::Type::Stat);
//		m_Mon_Ursacetus[i]->AddScript<Script_PheroDropper>(ScriptInfo::Type::PheroDropper);
//		m_Mon_Ursacetus[i]->AddScript<Script_DefaultEnemyBT>(ScriptInfo::Type::BehaviorTree);
//
//		m_Mon_Ursacetus[i]->Start();
//
//	}
//
//	//for (int i = 0; i < 10; ++i) {
//	//	m_Mon_AndCombat5[i] = std::make_shared<Monster_AdvancedCombatDroid_5>(i, Coordinate(0, 0));
//	//}
//
//	//for (int i = 0; i < 10; ++i) {
//	//	m_Mon_Onyscidus[i] = std::make_shared<Monster_Onyscidus>(i, Coordinate(0, 0));
//	//}
//
//	mHeightmapImage = std::make_shared<HeightMapImage>();
//	mHeightmapImage->Init("Contents/Resource/Terrain.bin");
//
//}
//
//void GameWorld::PQCS(OverlappedObject* over)
//{
//	::PostQueuedCompletionStatus(SERVER_NETWORK->GetIocpHandle(), 1, 0, over);
//}
