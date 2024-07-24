#include "pch.h"
#include "GamePhero.h"
#include "TimeManager.h"
#include "ServerLib/ThreadManager.h"

GamePhero::GamePhero(UINT32 id)
    : GameObject(id)
{
}

GamePhero::GamePhero()
    : GameObject()
{
}

GamePhero::~GamePhero()
{
}

void GamePhero::Update()
{
}

void GamePhero::WakeUp()
{
}

void GamePhero::Start()
{
}

void GamePhero::Activate()
{
	GameObject::Activate();

	mActivate_Ref.fetch_add(1);

	if (mInfo.PheroOwner == nullptr)
	{
		mInfo.PheroOwner = std::dynamic_pointer_cast<GamePhero>(shared_from_this());
	}
	if (mActivate_Ref.load() == 1) {
		TimerEvent t;
		t.Type = TimerEventType::Update_GameObject;
		t.WakeUp_Time = std::chrono::system_clock::now() + std::chrono::seconds(0); // ���� ���� ���� 
		t.Owner = shared_from_this();
		TIME_MGR->PushTimerEvent(t);
	}
}

void GamePhero::DeActivate()
{
	GameObject::DeActivate();

	mActivate_Ref.fetch_sub(1);

	LOG_MGR->Cout("DeActivate Ref : ", mActivate_Ref.load(), "\n");
}

void GamePhero::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);

	Update();

	if (GetActivate_RefCnt() > 0)
		GameObject::RegisterUpdate();
	else
	{

	}
}

SPtr<GamePhero> GamePhero::Clone()
{
    return SPtr<GamePhero>();
}
