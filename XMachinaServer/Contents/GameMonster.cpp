#include "pch.h"
#include "GameMonster.h"


void GameMonster::UpdateSnapShot()
{
	mInfo.ID         = GetID();
	mInfo.HP         = 100; // test
	mInfo.Attack     = 100;
	mInfo.Position   = GetTransform()->GetPosition();
	//mInfo.Rotation = GetTransform()->GetRotation();
}

GameMonster::GameMonster()
	: GameObject(-1)
{
}

GameMonster::GameMonster(UINT32 id, Coordinate sectorIdx)
	: GameObject(id)
{
	mSectorIndex = sectorIdx;
}

GameMonster::~GameMonster()
{
}

void GameMonster::Update()
{
	GameObject::Update();

	//LOG_MGR->Cout("Monster Update : ", static_cast<UINT8>(GetMonsterType()), "\n");


}

void GameMonster::WakeUp()
{
	GameObject::WakeUp();

}

void GameMonster::Start()
{
	GameObject::Start();

}

void GameMonster::Activate()
{
	GameObject::Activate();

	mActivate_Ref.fetch_add(1);

	if (mActivate_Ref.load() == 1) {
		TimerEvent t;
		t.Type        = TimerEventType::Update_GameObject;
		t.WakeUp_Time = std::chrono::system_clock::now() + std::chrono::seconds(0); // 지금 당장 시작 
		t.Owner       = shared_from_this();
		TIME_MGR->PushTimerEvent(t);
	}
}

void GameMonster::DeActivate()
{
	GameObject::DeActivate();

	mActivate_Ref.fetch_sub(1);

	LOG_MGR->Cout("DeActivate Ref : ", mActivate_Ref.load(), "\n");
}

void GameMonster::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);

	Update();
	
	if (GetActivate_RefCnt() > 0)
		GameObject::RegisterUpdate();
}

SPtr<GameMonster> GameMonster::Clone()
{
	SPtr<GameMonster> copy = std::make_shared<GameMonster>();
	
	copy->SetID(GetID());
	copy->SetMonsterType(this->GetMonsterType());
	copy->SetType(this->GetType());

	SPtr<GameObject> targetObj = std::dynamic_pointer_cast<GameObject>(copy);
	GameObject::CloneComponents(targetObj);
	GameObject::CloneScripts(targetObj);

	return copy;
}

void GameMonster::SetSectorIndex(Coordinate sectorIdx)
{
	mSectorIndex = sectorIdx;
}


