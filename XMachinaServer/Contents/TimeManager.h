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

#define DELTA_TIME TimeManager::GetInst()->GetTimeElapsed()
struct Timer {
	constexpr static ULONG kMaxSampleCnt = 50;		// Maximum frame time sample count

	double			DefaultTimeScale{};				// �⺻ �ð� �ӵ�
	double			TimeScale{};					// ���� �ð� �ӵ�
	float			TimeElapsed{};					// ���� �����ӿ��� ���� �����ӱ��� ��� �ð� (������ ��ȯ �ð�)

	/* performance count */
	__int64			BasePerfCnt{};
	__int64			PausedPerfCnt{};
	__int64			StopPerfCnt{};
	__int64			CurrPerfCnt{};
	__int64			LastPerfCnt{};

	float			FrameTime[kMaxSampleCnt]{};		// frame time�� �ִ� kMaxSampleCnt�� ���� ����Ѵ�.
	ULONG			SampleCnt{};					// ���� sampling�� frame cnt (�ִ� kMaxSampleCnt��)

	unsigned long	CurrFrameRate{};				// ���� frame rate
	unsigned long	FPS{};							// Tick() �Լ��� ȣ��� �� ���� �����Ѵ�. 1�ʰ� ����ϸ� 0���� �ʱ�ȭ�ȴ�.
	float			FPSTimeElapsed{};				// �� frame�� ����ð�

	bool			IsStopped{};					// �Ͻ����� �Ǿ��°�?
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

	// ������ ��ȯ �ð��� ��ȯ�Ѵ�.
	float GetTimeElapsed() const {
		return m_CurTimeElapsed;
	}//return m_Timer.TimeElapsed; }
	// ���� frame rate�� ���ڿ��� ��ȯ�Ѵ�.
	const WCHAR* GetFrameRate() const;

public:
	// calculate frame rate
	void Tick(float lockFPS = 0.f);

	// Ÿ�̸� ����(�簳)
	void Start();
	// Ÿ�̸� �Ͻ� ����
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

	// Ÿ�̸� �ʱ�ȭ
	void Reset();

};

