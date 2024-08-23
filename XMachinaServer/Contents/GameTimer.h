#pragma once

/// +-----------------------------------------------
///					 GameTimer 
/// ________________________________________________
///	- Create For Calculating Time
/// - To Use DeltaTime In Server Structure ( PQCS ) 
/// -----------------------------------------------+
/// 
class GameTimer
{
private:
	float mDeltaTime  = 0.f;
	bool  mActive     = false;
private:
	std::chrono::steady_clock::time_point	mCurrTimePoint = {};
	std::chrono::steady_clock::time_point	mPrevTimePoint = {};

public:
	GameTimer();
	~GameTimer();

public:
	void Start();
	void Update();

	void Activate();
	void DeActivate();

public:
	float	DeltaTime() { return mDeltaTime; }
	bool	IsActive() { return mActive; }
};

