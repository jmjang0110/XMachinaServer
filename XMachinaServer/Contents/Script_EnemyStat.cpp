#include "pch.h"
#include "Script_EnemyStat.h"
#include "GameObject.h"
#include "GameMonster.h"


Script_EnemyStat::Script_EnemyStat()
{
}

Script_EnemyStat::Script_EnemyStat(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Stat(owner , type)
{
}

Script_EnemyStat::~Script_EnemyStat()
{
}

void Script_EnemyStat::Clone(SPtr<Component> other) 
{
	Script::Clone(other);
	SPtr<Script_EnemyStat> otherScript = std::static_pointer_cast<Script_EnemyStat>(other);

	this->mEnemyLevel          = otherScript->mEnemyLevel;
	this->mPheroLevel          = otherScript->mPheroLevel;
	this->mMoveSpeed           = otherScript->mMoveSpeed;
	this->mRotationSpeed       = otherScript->mRotationSpeed;
	this->mAttackRotationSpeed = otherScript->mAttackRotationSpeed;
	this->mDetectionRange      = otherScript->mDetectionRange;
	this->mAttackRate          = otherScript->mAttackRate;
	this->mAttackRange         = otherScript->mAttackRange;
	this->mAttackCoolTime      = otherScript->mAttackCoolTime;
	this->mAttack1AnimName     = otherScript->mAttack1AnimName;
	this->mAttack2AnimName     = otherScript->mAttack2AnimName;
	this->mAttack3AnimName     = otherScript->mAttack3AnimName;

	SPtr<GameMonster> ownerMonster = std::dynamic_pointer_cast<GameMonster>(GetOwner());
	ownerMonster->SetEnemyStat(this);
}

void Script_EnemyStat::Activate()
{
	Script_Stat::Activate();
}

void Script_EnemyStat::DeActivate()
{
	Script_Stat::DeActivate();
}

bool Script_EnemyStat::WakeUp()
{
	Script_Stat::WakeUp();
	return true;
}

bool Script_EnemyStat::Start()
{
	Script_Stat::Start();
	return true;
}

bool Script_EnemyStat::Update()
{
	Script_Stat::Update();
	return true;
}

void Script_EnemyStat::OnDestroy()
{
	Script_Stat::OnDestroy();
}


bool Script_EnemyStat::Attack()
{
	return true;
}

void Script_EnemyStat::AttackCallback()
{
}

void Script_EnemyStat::Dead()
{
	Script_Stat::Dead();
}

bool Script_EnemyStat::Hit(float damage, SPtr_GameObject instigator)
{
	Script_Stat::Hit(damage, instigator);

	return false;
}
