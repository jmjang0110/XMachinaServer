#include "pch.h"

#undef max
#include "Enum_generated.h"

#include "Script_Enemy.h"
#include "Script_EnemyStat.h"
#include "GameObject.h"
#include "GameTimer.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"
#include "Script_EnemyStat.h"
#include "Script_BehaviorTrees.h"
#include "Script_Phero.h"
#include "Script_PheroDropper.h"

#include "Component.h"
#include "Transform.h"
#include "Animation.h"
#include "FBsPacketFactory.h"

#include "Script_EnemyController.h"
#include "GameRoom.h"
#include "RoomManager.h"

#include "DB_EnemyStat.h"

Script_Enemy::Script_Enemy()
{
}

Script_Enemy::Script_Enemy(SPtr<GameObject> owner)
	: Script_EnemyStat(owner)
{
	owner->EnableTag(ObjectTag::Enemy);

}

Script_Enemy::~Script_Enemy()
{
	mEnemyController = nullptr;
	
}

SPtr<Component> Script_Enemy::Clone(SPtr<Component> target)
{
	// 먼저, 기본 클래스인 Script_EnemyStat의 Clone을 호출

	// Script_Enemy 타입으로 캐스팅
	auto script_enemy = std::dynamic_pointer_cast<Script_Enemy>(target);
	if (script_enemy) {
		Script_EnemyStat::Clone(target);

		// Script_Enemy의 멤버들을 복사
		script_enemy->mAnimTime			= this->mAnimTime;
		script_enemy->mCurrAttackStep	= this->mCurrAttackStep;

		script_enemy->mSpawnPos			= this->mSpawnPos;
		script_enemy->mSpawnRot			= this->mSpawnRot;
		script_enemy->mPherosInfo		= this->mPherosInfo;
	}
	else {
		std::cout << "Clone failed: other is not of type Script_Enemy" << std::endl;
	}

	return target;
}

void Script_Enemy::Start()
{
	Script_EnemyStat::Start();

	mEnemyController          = mOwner->GetScript<Script_EnemyController>();
	auto Animation_Controller = OwnerAnimation()->GetController();

	if (mAttack1AnimName != "None") {
		Animation_Controller->FindMotionByName(mAttack1AnimName)->AddEndCallback(std::bind(&Script_Enemy::AttackEndCallback, this));
	}
	if (mAttack2AnimName != "None") {
		Animation_Controller->FindMotionByName(mAttack2AnimName)->AddEndCallback(std::bind(&Script_Enemy::AttackEndCallback, this));
	}
	if (mAttack3AnimName != "None") {
		Animation_Controller->FindMotionByName(mAttack3AnimName)->AddEndCallback(std::bind(&Script_Enemy::AttackEndCallback, this));
	}
	if (mDeathAnimName != "None") {
		Animation_Controller->FindMotionByName(mDeathAnimName)->AddEndCallback(std::bind(&Script_Enemy::DeathEndCallback, this));
	}

	mSpawnPos = mOwner->GetTransform()->GetPosition();
	mSpawnRot = Quaternion::ToEuler(mOwner->GetTransform()->GetRotation());

}

void Script_Enemy::Update()
{
	Script_EnemyStat::Update();

	mActiveTimer += mOwner->DeltaTime();

	if (mActiveTimer >= 1.f / 2.f) {
		Vec3 enemy_pos = mOwner->GetTransform()->GetPosition();
		Vec3 enemy_look = mOwner->GetTransform()->GetLook();
		auto spkt = FBS_FACTORY->SPkt_Monster_Transform(mOwner->GetID(), enemy_pos, enemy_look);
		ROOM_MGR->BroadcastRoom(mOwner->GetOwnerRoom()->GetID(), spkt);

	}

}

void Script_Enemy::LateUpdate()
{
}

void Script_Enemy::End()
{
}


void Script_Enemy::StartAttack()
{
	if (mCurrAttackStep == AttackType::None) {
		mCurrAttackStep = AttackType::BasicAttack;
	}

	mEnemyController->RemoveAllAnimation();
	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK);
	OwnerAnimation()->GetController()->SetValue("Attack", mCurrAttackStep);

	auto spkt = FBS_FACTORY->SPkt_Monster_State(mOwner->GetID(), FBProtocol::MONSTER_BT_TYPE_ATTACK, mCurrAttackStep);
	ROOM_MGR->BroadcastRoom(mOwner->GetOwnerRoom()->GetID(), spkt);
}

bool Script_Enemy::Attack()
{
	if (!Script_EnemyStat::Attack()) {
		return false;
	}

	return true;
}

void Script_Enemy::AttackCallback()
{
	if (!mEnemyController->GetTarget()) {
		return;
	}

	// TODO : 타겟 주변 레인지 범위 공격
	const Vec3& TargetPos = mEnemyController->GetTarget()->GetTransform()->GetSnapShot().GetPosition();
	const Vec3& Pos = OwnerTransform()->GetPosition();
	if (Vec3::Distance(TargetPos, Pos) <= mAttackRange) {
		const auto& statScript = mEnemyController->GetTarget()->GetScript<Script_Stat>();
		if (statScript) {

			statScript->Hit(mAttackRate, mOwner);
			Script_Stat::ObjectState state =  statScript->S_GetObjectState();
			if (state == Script_Stat::ObjectState::Dead) {
				mEnemyController->SetTarget(nullptr);
				mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
			}
		}
	}
}

void Script_Enemy::AttackEndCallback()
{
	mCurrAttackStep = AttackType::None;
	mEnemyController->RemoveAllAnimation();
	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
}

void Script_Enemy::DeathEndCallback()
{
	OwnerAnimation()->GetController()->GetCrntMotion()->SetSpeed(0.f);
}

void Script_Enemy::OnExitFromViewList()
{
	LOG_MGR->SetColor(TextColor::BrightCyan);
	LOG_MGR->Cout("[",mOwner->GetID(), "] - On Exit From View List \n");
	LOG_MGR->SetColor(TextColor::Default);

	mOwner->GetTransform()->SetPosition(mSpawnPos);
	mOwner->GetTransform()->SetLocalRotation(Quaternion::ToQuaternion(mSpawnRot));
	mOwner->GetTransform()->SyncSnapShot();
	
	if (mEnemyController) {
		mEnemyController->Reset();
		if (S_GetObjectState() == Script_Stat::ObjectState::Deactive)
			mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
	}

	auto spkt = FBS_FACTORY->SPkt_Monster_Transform(GetID(), mSpawnPos, mSpawnRot);
	ROOM_MGR->BroadcastRoom(mOwner->GetOwnerRoom()->GetID(), spkt);
}

void Script_Enemy::Dead()
{
	Script_EnemyStat::Dead();

}

bool Script_Enemy::Hit(float damage, SPtr<GameObject> instigator)
{
	bool res = Script_EnemyStat::Hit(damage, instigator);

	if (nullptr != instigator)
		mEnemyController->SetTarget(instigator);

	return res;
}


