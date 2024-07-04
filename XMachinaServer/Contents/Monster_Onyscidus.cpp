#include "pch.h"
#include "Monster_Onyscidus.h"
#include "TimeManager.h"
#include "ServerLib/MemoryManager.h"


Monster_Onyscidus::Monster_Onyscidus()
{
}

Monster_Onyscidus::Monster_Onyscidus(UINT32 sessionID)
	: GameMonster(sessionID)
{
	GameObject::SetType(GameObjectInfo::Type::Monster_Onyscidus) ;


}

Monster_Onyscidus::~Monster_Onyscidus()
{
}

void Monster_Onyscidus::Update()
{
}

void Monster_Onyscidus::WakeUp()
{
}

void Monster_Onyscidus::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);
	//return;

	TimerEvent t;
	t.Type = TimerEventType::Update_Onyscidus;
	t.WakeUp_Time = std::chrono::system_clock::now(); // 지금 당장 시작 
	t.Owner = shared_from_this();
	TIME_MGR->PushTimerEvent(t);

	speed += 1.f;

	//if ((int)speed % 10'000 == 0)
	//	LOG_MGR->Cout(this, " - ONYSCIDUS : ", speed, "\n");

}
