#include "pch.h"
#include "TimeManager.h"

DEFINE_SINGLETON(TimeManager);

void TimeManager::Launch()
{
	while (m_LaunchThread) {
		TimerEvent TEvent;
		auto CurTime = std::chrono::system_clock::now();

		/* ó���� Event �� ������. */
		if (true == m_TimerEventQueue.try_pop(TEvent)) {
			/* ���� ������ �ð��� �ȵǾ��ٸ� */
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
