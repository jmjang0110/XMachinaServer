#include "pch.h"
#include "Monster_Ursacetus.h"
#include "TimeManager.h"
#include "ServerLib/MemoryManager.h"
#include "ServerLib/ThreadManager.h"

Monster_Ursacetus::Monster_Ursacetus() 
	: GameMonster()
{
	GameObject::SetType(GameObjectInfo::Type::Monster_Ursacetus);

}

Monster_Ursacetus::Monster_Ursacetus(UINT32 sessionID, Coordinate sectorIdx)
	: GameMonster(sessionID, sectorIdx)
{

	GameObject::SetType(GameObjectInfo::Type::Monster_Ursacetus);

	GameMonster::SetID(sessionID);
	GameMonster::SetType(MonsterType::Ursacetus);
	GameMonster::SetAttack(100);
	GameMonster::SetHP(100);
	GameMonster::SetPosition(Vec3(25.f, 0.f, 260.f));
	GameMonster::SetRotation(Vec3(0.f, 0.f, 0.f));
	GameMonster::SetSpineDir(Vec3(0.f, 0.f, 0.f));

}

Monster_Ursacetus::~Monster_Ursacetus()
{
}

void Monster_Ursacetus::Init()
{
	Start();


}

void Monster_Ursacetus::Update()
{
	GameMonster::Update();

}

void Monster_Ursacetus::WakeUp()
{
	GameMonster::WakeUp();


}

void Monster_Ursacetus::Activate()
{
	GameMonster::Activate();

}

void Monster_Ursacetus::DeActivate()
{
	GameMonster::DeActivate();

}



void Monster_Ursacetus::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	
	MEMORY->Delete(overlapped);


	Update();
	//LOG_MGR->Cout(GetID(), " - Monster Ursacetus Updated\n");

	if (GameMonster::GetActivate_RefCnt() > 0)
		GameObject::RegisterUpdate();

	//if ((int)speed % 10'000 == 0)
		//LOG_MGR->Cout(this, " - URSACETUS : ", GetID(), " \n");

}


