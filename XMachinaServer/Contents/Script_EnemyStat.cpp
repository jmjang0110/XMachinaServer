#include "pch.h"
#include "Script_EnemyStat.h"

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
	Script_Stat::Clone(other);
	SPtr<Script_EnemyStat> otherScript = std::static_pointer_cast<Script_EnemyStat>(other);

	this->mEnemyLevel			= otherScript->mEnemyLevel			;
	this->mPheroLevel			= otherScript->mPheroLevel			;
	this->mMoveSpeed			= otherScript->mMoveSpeed			;
	this->mRotationSpeed		= otherScript->mRotationSpeed		;
	this->mAttackRotationSpeed	= otherScript->mAttackRotationSpeed	;
	this->mDetectionRange		= otherScript->mDetectionRange		;
	this->mAttackRate			= otherScript->mAttackRate			;
	this->mAttackRange			= otherScript->mAttackRange			;
	this->mAttackCoolTime		= otherScript->mAttackCoolTime		;

	this->mAttack1AnimName		= otherScript->mAttack1AnimName;
	this->mAttack2AnimName		= otherScript->mAttack2AnimName;
	this->mAttack3AnimName		= otherScript->mAttack3AnimName;

	this->mDeathAnimName		= otherScript->mDeathAnimName;
	this->mGetHitAnimName		= otherScript->mGetHitAnimName;

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

	return false;
}

bool Script_EnemyStat::Start()
{
	Script_Stat::Start();

	return false;
}

bool Script_EnemyStat::Update()
{
	Script_Stat::Update();

	return false;
}

void Script_EnemyStat::OnDestroy()
{
	Script_Stat::OnDestroy();

}

void Script_EnemyStat::Attack()
{

}

void Script_EnemyStat::AttackCallback()
{
}

void Script_EnemyStat::Dead()
{

}



bool Script_EnemyStat::Hit(float damage, SPtr_GameObject instigator)
{
	return false;
}
