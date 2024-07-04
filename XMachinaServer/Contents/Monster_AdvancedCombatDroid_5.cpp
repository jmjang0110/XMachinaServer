#include "pch.h"
#include "Monster_AdvancedCombatDroid_5.h"
#include "TimeManager.h"
#include "ServerLib/MemoryManager.h"

Monster_AdvancedCombatDroid_5::Monster_AdvancedCombatDroid_5()
{
}

Monster_AdvancedCombatDroid_5::Monster_AdvancedCombatDroid_5(UINT32 sessionID)
	: GameMonster(sessionID)
{
	GameObject::SetType(GameObjectInfo::Type::Monster_AdvancedCombat_5);

}

Monster_AdvancedCombatDroid_5::~Monster_AdvancedCombatDroid_5()
{
}

void Monster_AdvancedCombatDroid_5::Update()
{
}

void Monster_AdvancedCombatDroid_5::WakeUp()
{
}

void Monster_AdvancedCombatDroid_5::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);
	//return;

	TimerEvent t;
	t.Type        = TimerEventType::Update_AdvancedCombatDroid_5;
	t.WakeUp_Time = std::chrono::system_clock::now(); // 지금 당장 시작 
	t.Owner       = shared_from_this();
	TIME_MGR->PushTimerEvent(t);

	speed += 1.f;

	//if ((int)speed % 10'000 == 0)
	//	LOG_MGR->Cout(this, " - ADVCOM5 : ", speed, "\n");

}
