#pragma once

#define TIME_MGR TimeManager::GetInst()

enum class TimerEventType {
	None,
	Update_Ursacetus,

};

struct TimerEvent {
#pragma region Variable
	std::chrono::system_clock::time_point WakeUp_Time = {};
	TimerEventType Type{};
	SPtr_NetObj Owner{};
#pragma endregion

	constexpr bool operator<(const TimerEvent& L) const{ return (WakeUp_Time > L.WakeUp_Time);}
};

class TimeManager
{
	DECLARE_SINGLETON(TimeManager);

private:
	Concurrency::concurrent_priority_queue<TimerEvent> m_TimerEventQueue;
	bool m_LaunchThread = true;

public:
	void Launch();
	void Process_TimerEvent(TimerEvent ev);
	void PushTimerEvent(TimerEvent& ev);

public:
	TimeManager();
	~TimeManager();


};

