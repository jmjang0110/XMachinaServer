#include "pch.h"
#include "Monster_Ursacetus.h"
#include "TimeManager.h"


Monster_Ursacetus::Monster_Ursacetus() 
	: GameMonster()
{
}

Monster_Ursacetus::Monster_Ursacetus(UINT32 id) 
	: GameMonster(id)
{
}

Monster_Ursacetus::~Monster_Ursacetus()
{
}


void Monster_Ursacetus::Update()
{

}

void Monster_Ursacetus::WakeUp()
{

}

void Monster_Ursacetus::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	TimerEvent t;
	t.Type = TimerEventType::Update_Ursacetus;
	t.WakeUp_Time = std::chrono::system_clock::now(); // ���� ���� ���� 
	t.Owner = shared_from_this();
	TIME_MGR->PushTimerEvent(t);

	speed += 1.f;

	std::cout << shared_from_this().get() << " : SPEED : " << speed << std::endl;

}

