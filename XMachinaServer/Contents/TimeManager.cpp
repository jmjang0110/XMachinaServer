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
	__int64	perfFreq{};

	QueryPerformanceFrequency(perfFreq);
	m_Timer.DefaultTimeScale = 1.0 / static_cast<double>(perfFreq);
	m_Timer.TimeScale = m_Timer.DefaultTimeScale;

	Reset();
}

TimeManager::~TimeManager()
{
}


/// +---------------------------------
///				TIMER 
/// ---------------------------------+

float TimeManager::GetTotalTime() const
{
	return 0.0f;
}

const WCHAR* TimeManager::GetFrameRate() const
{
	return nullptr;
}

void TimeManager::Tick(float lockFPS)
{
	if (m_Timer.IsStopped) {
		m_Timer.TimeElapsed = 0.f;
		return;
	}

	QueryPerformanceCounter(m_Timer.CurrPerfCnt);
	float timeElapsed = static_cast<float>((m_Timer.CurrPerfCnt - m_Timer.LastPerfCnt) * m_Timer.TimeScale);

	if (lockFPS > 0.f) {
		while (timeElapsed < (1.f / lockFPS)) {
			QueryPerformanceCounter(m_Timer.CurrPerfCnt);
			timeElapsed = static_cast<float>((m_Timer.CurrPerfCnt - m_Timer.LastPerfCnt) * m_Timer.TimeScale);
		}
	}

	m_Timer.LastPerfCnt = m_Timer.CurrPerfCnt;

	if (fabsf(timeElapsed - m_Timer.TimeElapsed) < 1.f) {
		::memmove(&m_Timer.FrameTime[1], m_Timer.FrameTime, (m_Timer.kMaxSampleCnt - 1) * sizeof(float));
		m_Timer.FrameTime[0] = timeElapsed;
		if (m_Timer.SampleCnt < m_Timer.kMaxSampleCnt) {
			m_Timer.SampleCnt++;
		}
	}

	m_Timer.FPS++;
	m_Timer.FPSTimeElapsed += timeElapsed;
	if (m_Timer.FPSTimeElapsed > 1.f) {
		m_Timer.CurrFrameRate = m_Timer.FPS;
		m_Timer.FPS = 0;
		m_Timer.FPSTimeElapsed = 0.f;
	}

	m_Timer.TimeElapsed = 0.f;
	for (ULONG i = 0; i < m_Timer.SampleCnt; i++) {
		m_Timer.TimeElapsed += m_Timer.FrameTime[i];
	}
	if (m_Timer.SampleCnt > 0) {
		m_Timer.TimeElapsed /= m_Timer.SampleCnt;
	}
}

void TimeManager::Start()
{
	__int64 perfCnt;
	QueryPerformanceCounter(perfCnt);
	if (m_Timer.IsStopped) {
		m_Timer.PausedPerfCnt += (perfCnt - m_Timer.StopPerfCnt);
		m_Timer.LastPerfCnt    = perfCnt;
		m_Timer.StopPerfCnt    = 0;
		m_Timer.IsStopped      = false;
	}
}

void TimeManager::Stop()
{
	if (!m_Timer.IsStopped)
	{
		QueryPerformanceCounter(m_Timer.StopPerfCnt);
		m_Timer.IsStopped = true;
	}
}

void TimeManager::SetTimeScale(double scale)
{
	m_Timer.TimeScale = m_Timer.DefaultTimeScale * scale;

}

void TimeManager::Reset()
{
	__int64 perfCnt;
	QueryPerformanceCounter(perfCnt);

	m_Timer.BasePerfCnt = perfCnt;
	m_Timer.LastPerfCnt = perfCnt;
	m_Timer.StopPerfCnt = 0;
	m_Timer.IsStopped = false;
}
