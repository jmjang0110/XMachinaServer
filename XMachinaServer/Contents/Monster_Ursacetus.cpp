#include "pch.h"
#include "Monster_Ursacetus.h"
#include "TimeManager.h"
#include "ServerLib/MemoryManager.h"
#include "ServerLib/ThreadManager.h"

Monster_Ursacetus::Monster_Ursacetus() 
	: GameMonster()
{
}

Monster_Ursacetus::Monster_Ursacetus(UINT32 sessionID, Coordinate sectorIdx)
	: GameMonster(sessionID, sectorIdx)
{

	GameObject::SetType(GameObjectInfo::Type::Monster_Ursacetus);

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


	GameObject::RegisterUpdate();

	//if ((int)speed % 10'000 == 0)
	//	LOG_MGR->Cout(this, " - URSACETUS : ", speed, "\n");

}


