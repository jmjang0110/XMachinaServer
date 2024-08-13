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
#include "Script_EnemyController.h"
#include "Transform.h"

#include "Script_Phero.h"
#include "Script_PheroDropper.h"
#include "GamePhero.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Anglerox.h"
#include "Script_Arack.h"
#include "Script_Aranobot.h"
#include "Script_Ceratoferox.h"
#include "Script_Gobbler.h"
#include "Script_LightBipedMech.h"
#include "Script_MiningMech.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
#include "Script_Rapax.h"

#include "BTTask.h"

const std::vector<SPtr<GamePhero>>& GameMonster::GetAllPheros()
{
	const auto& script = GetScript<Script_PheroDropper>(ScriptInfo::Type::PheroDropper);
	const std::vector<SPtr<GamePhero>>& pheros = script->GetPheros();
	return pheros;
}

void GameMonster::UpdateSnapShot()
{
	mInfo.ID         = GetID();
}

void GameMonster::On_ExitFromViewList()
{
	//LOG_MGR->Cout(GetID(), " - On Exit From view List ");

	GetTransform()->SetPosition(mSpawnPos);

	/* Update Snap Shot [0]  */
	GetTransform()->UpdateTransofrmSnapShot();
	GetTransform()->SwapSnapShotIndex();

	/* Update Snap Shot [1]  */
	GetTransform()->UpdateTransofrmSnapShot();
	GetTransform()->SwapSnapShotIndex();


	GetTransform()->SetLocalRotation(Quaternion::ToQuaternion(mSpawnRot));
	mEnemyController->Reset();

	if(S_GetObjectState() == Script_Stat::ObjectState::Deactive)
		mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE::MONSTER_BT_TYPE_IDLE);


	auto spkt = FBS_FACTORY->SPkt_Monster_Transform(GetID(), mSpawnPos, mSpawnRot);
	GAME_MGR->BroadcastRoom(GetOwnerNPCController()->GetOwnerRoom()->GetID(), spkt);
}

void GameMonster::OnHit(int hitCnt)
{
	if(mEnemyStat->S_GetObjectState() == Script_Stat::ObjectState::Dead)
		return;


	// TODO : TEST Hit Cnt == 5 -> Dead 
	HitCnt += hitCnt;
	//LOG_MGR->Cout(GetID(), " : OnHit\n");
	if (HitCnt >= 5) {
		mEnemyStat->S_SetObjectState(Script_Stat::ObjectState::Dead);
	}

	//// TODO : Test : OnHit ( Mosnter <--> Bullet ) 
	//if (mEnemyStat->S_GetObjectState() == Script_Stat::ObjectState::Dead) {
	//	LOG_MGR->Cout(GetID(), " : Dead\n");
	//	Vec3 pos = GetTransform()->GetSnapShot().GetPosition();

	//	auto pheroDropper = GetScript<Script_PheroDropper>(ScriptInfo::Type::PheroDropper);
	//	pheroDropper->SetPherosPos(pos);

	//	if (mSendDeadMonsterPkt.load() == false) {
	//		mSendDeadMonsterPkt = true;
	//		/// > ¢¹ Send Dead Monster Info ( Dead Point(pos), Pheros Info(string) )
	//		auto spkt = FBS_FACTORY->SPkt_DeadMonster(GetID(), pos, GetPheros());
	//		GAME_MGR->BroadcastRoom(GetOwnerNPCController()->GetOwnerRoom()->GetID(), spkt);
	//	}

	//	return;
	//}

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

	
	mTimer += GetDeltaTime();
	if (mTimer >= 1.f / 2.f) {

		/* Send Transform Packet */
		Vec3 Pos  = GetTransform()->GetPosition();
		auto spkt = FBS_FACTORY->SPkt_Monster_Transform(GetID(), Pos, GetTransform()->GetLook());
		GAME_MGR->BroadcastRoom(GetOwnerNPCController()->GetOwnerRoom()->GetID(), spkt);
		
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

	mSpawnPos = GetTransform()->GetPosition();
	mSpawnRot = Quaternion::ToEuler(GetTransform()->GetRotation());
}

void GameMonster::Activate()
{
	mActivate_Ref.fetch_add(1);

	if (mInfo.owner == nullptr)
	{
		mInfo.owner = std::dynamic_pointer_cast<GameMonster>(shared_from_this());
	}
	if (mActivate_Ref.load() == 1) {
		S_SetObjectState(Script_Stat::ObjectState::Active);
		GameObject::Activate();
		GameObject::RegisterUpdate();
	}
}

void GameMonster::DeActivate()
{
	mActivate_Ref.fetch_sub(1);

	if (mActivate_Ref.load() == 0) {
		GameObject::DeActivate();
		S_SetObjectState(Script_Stat::ObjectState::Deactive);

	}

	//LOG_MGR->Cout("[", GetID(), "] : DeActivate Ref : ", mActivate_Ref.load(), "\n");
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

ScriptInfo::Type GameMonster::GetScriptInfoType() {
	FBProtocol::MONSTER_TYPE type = mInfo.Type;
	ScriptInfo::Type t = ScriptInfo::Type::Stat;
	return t;
}

Script_Enemy* GameMonster::GetEnemyScript() {
	ScriptInfo::Type type = GetScriptInfoType();
	Script_Enemy* script = GetScript<Script_Enemy>(type).get();
	return script;
}

SPtr<GamePlayer> GameMonster::GetInvoker()
{
	return mEnemyController->GetInvoker();
}

