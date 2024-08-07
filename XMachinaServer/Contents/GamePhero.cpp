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
	GameObject::Update();

}

void GamePhero::WakeUp()
{
	GameObject::WakeUp();

}

void GamePhero::Start()
{
	GameObject::Start();

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
		t.WakeUp_Time = std::chrono::system_clock::now() + std::chrono::seconds(0); // 지금 당장 시작 
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

}

SPtr<GamePhero> GamePhero::Clone()
{
	SPtr<GamePhero> copy       = std::make_shared<GamePhero>();
	
	copy->mInfo                = mInfo;
	
	SPtr<GameObject> targetObj = std::dynamic_pointer_cast<GameObject>(copy);
	GameObject::CloneComponents(targetObj);
	GameObject::CloneScripts(targetObj);

    return copy;
}
