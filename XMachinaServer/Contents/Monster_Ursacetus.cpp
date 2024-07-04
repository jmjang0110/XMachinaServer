#include "pch.h"
#include "Monster_Ursacetus.h"
#include "TimeManager.h"
#include "ServerLib/MemoryManager.h"


Monster_Ursacetus::Monster_Ursacetus() 
	: GameMonster()
{
}

Monster_Ursacetus::Monster_Ursacetus(UINT32 sessionID)
	: GameMonster(sessionID)
{

	GameObject::SetType(GameObjectInfo::Type::Monster_Ursacetus);

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
	
	MEMORY->Delete(overlapped);
	//return;

	TimerEvent t;
	t.Type = TimerEventType::Update_Ursacetus;
	t.WakeUp_Time = std::chrono::system_clock::now(); // 지금 당장 시작 
	t.Owner = shared_from_this();
	TIME_MGR->PushTimerEvent(t);


	//if ((int)speed % 10'000 == 0)
	//	LOG_MGR->Cout(this, " - URSACETUS : ", speed, "\n");

}

