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

	mEnemyController = std::dynamic_pointer_cast<Script_EnemyController>(GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController));

	if (GetStat_Attack1AnimName() != "None") {
		 GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddEndCallback(std::bind(&Script_Enemy::AttackEndCallback, this));
	}
	if (GetStat_Attack2AnimName() != "None") {
		GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack2AnimName())->AddEndCallback(std::bind(&Script_Enemy::AttackEndCallback, this));
	}
	if (GetStat_Attack3AnimName() != "None") {
		GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack3AnimName())->AddEndCallback(std::bind(&Script_Enemy::AttackEndCallback, this));
	}
	if (GetStat_DeathAnimName() != "None") {
		GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_DeathAnimName())->AddEndCallback(std::bind(&Script_Enemy::DeathEndCallback, this));
	}

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

void Script_Enemy::StartAttack()
{
	if (mCurrAttackCnt == AttackType::None) {
		mCurrAttackCnt = AttackType::BasicAttack;
	}

	mEnemyController->RemoveAllAnimation();
	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK);
	GetOwner()->GetAnimation()->GetController()->SetValue("Attack", mCurrAttackCnt);
	std::cout << mCurrAttackCnt << std::endl;
}

bool Script_Enemy::Attack()
{
	if (!Script_EnemyStat::Attack()) {
		return false;
	}

	if (!mEnemyController->GetTarget()) {
		return false;
	}

	//// TODO : Ÿ�� �ֺ� ������ ���� ����
	//const Vec3& TargetPos = mEnemyController->GetTarget()->GetTransform()->GetSnapShot().GetPosition();
	//const Vec3& Pos = GetOwner()->GetTransform()->GetPosition();
	//if (Vec3::Distance(TargetPos, Pos) <= GetStat_AttackRange()) {

	//	const auto& statScript = mEnemyController->GetTarget()->GetScript<Script_Stat>(ScriptInfo::Type::Stat);
	//	if (statScript) {
	//		statScript->Hit(GetStat_AttackRate(), nullptr);
	//	}
	//}

	return true;
}

void Script_Enemy::AttackEndCallback()
{
	mCurrAttackCnt = AttackType::None;
	mEnemyController->RemoveAllAnimation();
	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
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

	if (nullptr != instigator)
		mEnemyController->SetTarget(instigator);
	return res;
}

