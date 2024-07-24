#include "pch.h"
#include "GameBullet.h"

GameBullet::GameBullet()
	: GameObject()
{
}

GameBullet::GameBullet(UINT32 sessionID)
	: GameObject(sessionID)
{
}

GameBullet::GameBullet(UINT32 sessionID, SPtr<GamePlayer> owner)
	: GameObject(sessionID)
{
	mOwnerPlayer = owner;

}

GameBullet::~GameBullet()
{
}

void GameBullet::Update()
{

}

void GameBullet::WakeUp()
{
}

void GameBullet::Start()
{
}

void GameBullet::Activate()
{
	GameObject::Activate();

	mActivate_Ref.fetch_add(1);

	if (mInfo.BulletOwner == nullptr)
	{
		mInfo.BulletOwner = std::dynamic_pointer_cast<GameBullet>(shared_from_this());
	}
	if (mActivate_Ref.load() == 1) {
		TimerEvent t;
		t.Type        = TimerEventType::Update_GameObject;
		t.WakeUp_Time = std::chrono::system_clock::now() + std::chrono::seconds(0); // 지금 당장 시작 
		t.Owner       = shared_from_this();
		TIME_MGR->PushTimerEvent(t);
	}
}

void GameBullet::DeActivate()
{
}

void GameBullet::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);

	Update();

	if (GetActivate_RefCnt() > 0)
		GameObject::RegisterUpdate();
	else
	{

	}
}

SPtr<GameBullet> GameBullet::Clone()
{
	return SPtr<GameBullet>();
}
