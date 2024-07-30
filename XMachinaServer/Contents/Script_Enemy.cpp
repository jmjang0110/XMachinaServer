#include "pch.h"

#undef max
#include "Enum_generated.h"

#include "Script_Enemy.h"
#include "Script_EnemyStat.h"
#include "GameObject.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"
#include "Script_EnemyStat.h"
#include "Script_DefaultEnemyBT.h"
#include "Script_Phero.h"
#include "Script_PheroDropper.h"

#include "Component.h"
#include "Transform.h"



Script_Enemy::Script_Enemy()
{
}

Script_Enemy::Script_Enemy(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_EnemyStat(owner, type)
{
	mEnemyController = std::dynamic_pointer_cast<Script_EnemyController>(GetOwner()->AddScript<Script_EnemyController>(ScriptInfo::Type::EnemyController));

}

Script_Enemy::~Script_Enemy()
{
	mEnemyController = nullptr;

}

void Script_Enemy::Clone(SPtr<Component> other) 
{
	Script_EnemyStat::Clone(other);
	SPtr<Script_Enemy> otherScript = std::static_pointer_cast<Script_Enemy>(other);

}

void Script_Enemy::Activate()
{
	Script_EnemyStat::Activate();

}

void Script_Enemy::DeActivate()
{
	Script_EnemyStat::DeActivate();

}


bool Script_Enemy::WakeUp()
{
	Script_EnemyStat::WakeUp();



    return true;
}

bool Script_Enemy::Start()
{
	Script_EnemyStat::Start();

	mEnemyController = std::dynamic_pointer_cast<Script_EnemyController>(GetOwner()->AddScript<Script_EnemyController>(ScriptInfo::Type::EnemyController));
	//GetOwner()->AddScript<Script_PheroDropper>(ScriptInfo::Type::PheroDropper);
	//GetOwner()->AddScript<Script_DefaultEnemyBT>(ScriptInfo::Type::BehaviorTree);

    return true;
}

bool Script_Enemy::Update()
{
	Script_EnemyStat::Update();

	return true;
}

void Script_Enemy::OnDestroy()
{
	Script_EnemyStat::OnDestroy();

}

void Script_Enemy::Attack()
{
	Script_EnemyStat::Attack();

	mAnimTime += GetOwner()->GetDeltaTime();

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK_1);
	mEnemyController->GetOwnerMonster()->SetBTState(FBProtocol::MONSTER_BT_TYPE_ATTACK_1);
}

void Script_Enemy::AttackCallback()
{
	if (!mEnemyController->GetTarget()) {
		return;
	}

	// TODO : 타겟 주변 레인지 범위 공격
	const Vec3& TargetPos = mEnemyController->GetTarget()->GetTransform()->GetSnapShot().GetPosition();
	const Vec3& Pos = GetOwner()->GetTransform()->GetPosition();
	if (Vec3::Distance(TargetPos, Pos) <= GetStat_AttackRange()) {

		const auto& statScript = mEnemyController->GetTarget()->GetScript<Script_Stat>(ScriptInfo::Type::Stat);
		if (statScript) {
			// statScript->Hit(GetStat_AttackRate(), nullptr);
		}
	}
}

void Script_Enemy::AttackEndCallback()
{
	mEnemyController->SetState(EnemyInfo::State::Idle);

}

void Script_Enemy::DeathEndCallback()
{
	GetOwner()->GetAnimation()->GetController()->GetCrntMotion()->SetSpeed(0.f);
}

void Script_Enemy::Dead()
{
	Script_EnemyStat::Dead();

}

bool Script_Enemy::Hit(float damage, SPtr_GameObject instigator)
{

	bool res = Script_EnemyStat::Hit(damage, instigator);


	if (nullptr != instigator) {
		mEnemyController->SetTarget(instigator);

		//if (instigator->GetType() == GameObjectInfo::Type::GamePlayer) {
		//	mEnemyController->SetTargetPlayer(std::dynamic_pointer_cast<GamePlayer>(instigator));
		//}
		//else if (instigator->GetType() == GameObjectInfo::Type::Monster_AdvancedCombat_5
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_Anglerox
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_Arack
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_Aranobot
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_Ceratoferox
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_Gobbler
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_LightBipedMech
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_MiningMech
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_Rapax
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_Onyscidus
		//	|| instigator->GetType() == GameObjectInfo::Type::Monster_Ursacetus) {
		//	mEnemyController->SetTargetMonster(std::dynamic_pointer_cast<GameMonster>(instigator));
		//}
	}

	return res;
}

