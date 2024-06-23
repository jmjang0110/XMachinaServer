#include "pch.h"
#include "TimeManager.h"
#include "GameOverlapped.h"
#include "ServerLib/MemoryManager.h"
#include "GameWorld.h"

DEFINE_SINGLETON(TimeManager);

void TimeManager::Launch()
{
	while (m_LaunchThread) {
		TimerEvent TEvent;
		auto CurTime = std::chrono::system_clock::now();

		/* 처리할 Event 를 꺼낸다. */
		if (true == m_TimerEventQueue.try_pop(TEvent)) {
			
			/* 아직 실해할 시간이 안되었다면 */
			if (TEvent.WakeUp_Time > CurTime) {
				m_TimerEventQueue.push(TEvent);
				continue;
			}

			/* Timer Event 실행 */
			Process_TimerEvent(TEvent);
		}
	}
}

void TimeManager::Process_TimerEvent(TimerEvent ev)
{

	switch (ev.Type)
	{
	case TimerEventType::Update_Ursacetus:
	{
	
		Overlapped_Monster_Update* over = MEMORY->New<Overlapped_Monster_Update>();
		over->SetOwner(ev.Owner);
		GAME_WORLD->PQCS(reinterpret_cast<OverlappedObject*>(over));

	}
	break;

	default:
		break;
	}


}

void TimeManager::PushTimerEvent(TimerEvent& ev)
{
	m_TimerEventQueue.push(ev);
}


TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}
