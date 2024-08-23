#include "pch.h"
#include "GameTimer.h"

GameTimer::GameTimer()
{
}

GameTimer::~GameTimer()
{
}

void GameTimer::Activate()
{
	mActive        = true;
	Start();
}

void GameTimer::DeActivate()
{
	mActive = false;
	mDeltaTime = 0.f;
}

void GameTimer::Start()
{
	mDeltaTime = 0.f;
	mCurrTimePoint = std::chrono::steady_clock::now();

}

void GameTimer::Update()
{
	if (!mActive)
		return;

	mPrevTimePoint = mCurrTimePoint;
	mCurrTimePoint = std::chrono::steady_clock::now();

	std::chrono::duration<double> elapsed_seconds = mCurrTimePoint - mPrevTimePoint;
	mDeltaTime = static_cast<float>(elapsed_seconds.count());
}
