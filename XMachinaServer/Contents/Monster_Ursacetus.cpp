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
	t.WakeUp_Time = std::chrono::system_clock::now(); // 지금 당장 시작 
	t.Owner = shared_from_this();
	TIME_MGR->PushTimerEvent(t);

	speed += 1.f;


}

