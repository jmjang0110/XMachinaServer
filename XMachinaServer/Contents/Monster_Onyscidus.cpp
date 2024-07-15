#include "pch.h"
#include "Monster_Onyscidus.h"
#include "TimeManager.h"
#include "ServerLib/MemoryManager.h"


Monster_Onyscidus::Monster_Onyscidus()
{
}

Monster_Onyscidus::Monster_Onyscidus(UINT32 sessionID, Coordinate sectorIdx)
	: GameMonster(sessionID, sectorIdx)
{
	GameObject::SetType(GameObjectInfo::Type::Monster_Onyscidus) ;

	GameMonster::SetID(sessionID);
	GameMonster::SetType(MonsterType::Onyscidus);
	GameMonster::SetAttack(100);
	GameMonster::SetHP(100);
	GameMonster::SetPosition(Vec3(0.f, 0.f, 0.f));
	GameMonster::SetRotation(Vec3(0.f, 0.f, 0.f));
	GameMonster::SetSpineDir(Vec3(0.f, 0.f, 0.f));
}

Monster_Onyscidus::~Monster_Onyscidus()
{
}

void Monster_Onyscidus::Init()
{
}

void Monster_Onyscidus::Update()
{
	GameMonster::Update();
	std::cout << "Monster_Onyscidus\n";

}

void Monster_Onyscidus::WakeUp()
{
}

void Monster_Onyscidus::Activate()
{
}

void Monster_Onyscidus::DeActivate()
{
}

void Monster_Onyscidus::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);

	Update();

	if (GameMonster::GetActivate_RefCnt() > 0)
		GameObject::RegisterUpdate();

	//if ((int)speed % 10'000 == 0)
	//	LOG_MGR->Cout(this, " - ONYSCIDUS : ", speed, "\n");

}
