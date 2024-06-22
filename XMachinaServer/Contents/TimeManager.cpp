#include "pch.h"
#include "TimeManager.h"

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

			switch (TEvent.Type)
			{
			default:
				break;
			}
		}


	}

}

TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}
