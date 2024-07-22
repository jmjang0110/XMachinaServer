#pragma once

#define TIME_MGR TimeManager::GetInst()

enum class TimerEventType {
	None,

	Update_GameObject,

	END,

};

struct TimerEvent {
#pragma region Variable
	std::chrono::system_clock::time_point WakeUp_Time = {};
	TimerEventType Type{};
	SPtr_NetObj Owner{};
#pragma endregion

	constexpr bool operator<(const TimerEvent& L) const{ return (WakeUp_Time > L.WakeUp_Time);}
};

static float g_ElapsedTime = 0.f;

struct Timer {
	constexpr static ULONG kMaxSampleCnt = 50;		// Maximum frame time sample count

	double			DefaultTimeScale{};				// 기본 시간 속도
	double			TimeScale{};					// 현재 시간 속도
	float			TimeElapsed{};					// 이전 프레임에서 현재 프레임까지 경과 시간 (프레임 전환 시간)

	/* performance count */
	__int64			BasePerfCnt{};
	__int64			PausedPerfCnt{};
	__int64			StopPerfCnt{};
	__int64			CurrPerfCnt{};
	__int64			LastPerfCnt{};

	float			FrameTime[kMaxSampleCnt]{};		// frame time을 최대 kMaxSampleCnt개 까지 기록한다.
	ULONG			SampleCnt{};					// 현재 sampling된 frame cnt (최대 kMaxSampleCnt개)

	unsigned long	CurrFrameRate{};				// 현재 frame rate
	unsigned long	FPS{};							// Tick() 함수가 호출될 때 마다 증가한다. 1초가 경과하면 0으로 초기화된다.
	float			FPSTimeElapsed{};				// 한 frame의 경과시간

	bool			IsStopped{};					// 일시정지 되었는가?
};


class TimeManager
{
	DECLARE_SINGLETON(TimeManager);

private:
	Concurrency::concurrent_priority_queue<TimerEvent> m_TimerEventQueue;
	bool m_LaunchThread = true;

	Timer m_Timer;
	double m_Time{};

	float m_CurTimeElapsed{};
public:
	void Launch();
	void Process_TimerEvent(TimerEvent ev);
	void PushTimerEvent(TimerEvent& ev);

public:
	TimeManager();
	~TimeManager();

	/// +---------------------------------
	///				TIMER 
	/// ---------------------------------+

public:
	float GetTotalTime() const;

	// 프레임 전환 시간를 반환한다.
	float GetTimeElapsed() const {
		return m_CurTimeElapsed;
	}//return m_Timer.TimeElapsed; }
	// 현재 frame rate를 문자열로 반환한다.
	const WCHAR* GetFrameRate() const;

public:
	// calculate frame rate
	void Tick(float lockFPS = 0.f);

	// 타이머 시작(재개)
	void Start();
	// 타이머 일시 정지
	void Stop();

	void SetTimeScale(double scale);

private:
	void QueryPerformanceFrequency(__int64& freq)
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	}
	void QueryPerformanceCounter(__int64& cnt)
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&cnt);
	}

	// 타이머 초기화
	void Reset();

};

