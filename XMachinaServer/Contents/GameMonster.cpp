#include "pch.h"
#include "GameMonster.h"
#include "GameObject.h"
#include "NPCController.h"
#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"
#include "GamePlayer.h"
#include "CollisionManager.h"
#include "PlayerController.h"
#include "Script_PheroDropper.h"
#include "FBsPacketFactory.h"
#include "GameManager.h"
#include "TimeManager.h"
#include "ServerLib/ThreadManager.h"

const std::vector<SPtr<GameObject>>& GameMonster::GetAllPheros()
{
	const auto& script = GetScript<Script_PheroDropper>(ScriptInfo::Type::PheroDropper);
	const std::vector<SPtr<GameObject>>& pheros = script->GetPheros();
	return pheros;
}

void GameMonster::UpdateSnapShot()
{
	mInfo.ID         = GetID();
	mInfo.HP         = 100; // test
	mInfo.Attack     = 100;
	mInfo.Position   = GetTransform()->GetPosition();
	//mInfo.Rotation = GetTransform()->GetRotation();
}

void GameMonster::On_ExitFromViewList()
{
	LOG_MGR->Cout(GetID(), " - On Exit From view List ");
}

void GameMonster::Broadcast_SPkt_Monster_Transform()
{	
	// Transform 패킷 보내기 
	Vec3 Pos  = GetTransform()->GetPosition();
	Vec3 Rot  = Quaternion::ToEuler(GetTransform()->GetRotation());
	auto spkt = FBS_FACTORY->SPkt_Monster_Transform(GetID(), Pos, Rot);

	GAME_MGR->BroadcastRoom(GetOwnerNPCController()->GetOwnerRoom()->GetID(), spkt);
}

void GameMonster::Broadcast_SPkt_Mosnter_State(FBProtocol::MONSTER_BT_TYPE monster_bt_type)
{
	auto spkt = FBS_FACTORY->SPkt_Monster_State(GetID(), monster_bt_type);

	GAME_MGR->BroadcastRoom(GetOwnerNPCController()->GetOwnerRoom()->GetID(), spkt);
}

void GameMonster::Send_SPkt_Mosnter_State(FBProtocol::MONSTER_BT_TYPE monser_bt_type)
{
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
	GameObject::Update(); // Component, SCript Update

	mTimer += DELTA_TIME;
	// 0.5초마다 패킷 전송
	if (mTimer >= 0.1f) {
		/* Send Transform Packet */
		Broadcast_SPkt_Monster_Transform();

		Vec3 pos = GetTransform()->GetPosition();

		//LOG_MGR->Cout(GetName(), " : ", GetID(), " : ", pos.x , " ", pos.y, " ", pos.z, "\n");

		// 타이머 초기화
		mTimer = 0.0f;
	}
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

	if (mInfo.owner == nullptr)
	{
		mInfo.owner = std::dynamic_pointer_cast<GameMonster>(shared_from_this());
	}
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
	else
	{
		if (mInfo.owner) {
			mInfo.owner->On_ExitFromViewList();
		}
	}
}

SPtr<GameMonster> GameMonster::Clone()
{
	SPtr<GameMonster> copy = std::make_shared<GameMonster>();
	
	copy.get()->mInfo       = mInfo;
	copy.get()->mInfo.owner = copy;
	copy.get()->mInfo.ID = mInfo.ID;
	
	copy->SetID(GetID());
	copy->SetType(this->GetType());

	SPtr<GameObject> targetObj = std::dynamic_pointer_cast<GameObject>(copy);
	GameObject::CloneComponents(targetObj);
	GameObject::CloneScripts(targetObj);

	copy->SetPheros(mInfo.Pheros);
	

	return copy;
}

void GameMonster::SetSectorIndex(Coordinate sectorIdx)
{
	mSectorIndex = sectorIdx;
}


