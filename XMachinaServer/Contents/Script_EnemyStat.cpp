#include "pch.h"
#include "Script_EnemyStat.h"
#include "GameObject.h"
#include "DB_EnemyStat.h"
#include "ResourceManager.h"

Script_EnemyStat::Script_EnemyStat()
{
    mKey = uint32_t_ScriptKey(ScriptKey::Enemy);
}

Script_EnemyStat::Script_EnemyStat(SPtr<GameObject> owner)
	: Script_Stat(owner, uint32_t_ScriptKey(ScriptKey::Enemy))
{
}

Script_EnemyStat::~Script_EnemyStat()
{
}

SPtr<Component> Script_EnemyStat::Clone(SPtr<Component> target)
{
    // 먼저, 기본 클래스인 Script_Stat의 Clone을 호출
    Script_Stat::Clone(target);

    // Cast the target to the appropriate type (Script_EnemyStat)
    auto enemyStat = std::dynamic_pointer_cast<Script_EnemyStat>(target);

    // Ensure the casting was successful
    if (enemyStat)
    {
        // Copy the Script_EnemyStat-specific member variables
        enemyStat->mType                = this->mType;
        enemyStat->mEnemyLevel          = this->mEnemyLevel;
        enemyStat->mPheroLevel          = this->mPheroLevel;
        enemyStat->mMoveSpeed           = this->mMoveSpeed;
        enemyStat->mRotationSpeed       = this->mRotationSpeed;
        enemyStat->mAttackRotationSpeed = this->mAttackRotationSpeed;
        enemyStat->mDetectionRange      = this->mDetectionRange;
        enemyStat->mAttackRate          = this->mAttackRate;
        enemyStat->mAttackRange         = this->mAttackRange;
        enemyStat->mAttackCoolTime      = this->mAttackCoolTime;
        enemyStat->mAttack1AnimName     = this->mAttack1AnimName;
        enemyStat->mAttack2AnimName     = this->mAttack2AnimName;
        enemyStat->mAttack3AnimName     = this->mAttack3AnimName;
        enemyStat->mGetHitAnimName      = this->mGetHitAnimName;
        enemyStat->mDeathAnimName       = this->mDeathAnimName;
    }

    return target;
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

bool Script_EnemyStat::Hit(float damage, SPtr<GameObject> instigator)
{
	Script_Stat::Hit(damage, instigator);

	return false;
}

void Script_EnemyStat::SetDataFromDataBase(std::string PK_EnemyName)
{
    auto DB = RESOURCE_MGR->GetEnemyStatInfo(PK_EnemyName);
    
    mEnemyLevel          = DB->Level;
    mPheroLevel          = DB->PheroLevel;
    
    mMoveSpeed           = DB->MoveSpeed;
    mRotationSpeed       = DB->RotationSpeed;
    mAttackRotationSpeed = DB->AttackRotationSpeed;

    mDetectionRange      = DB->DetectionRange;
    mAttackRate          = DB->AttackRate;
    mAttackRange         = DB->AttackRange;
    mAttackCoolTime      = DB->AttackCoolTime;
                         
    mAttack1AnimName     = DB->Attack1AnimName;
    mAttack2AnimName     = DB->Attack2AnimName; 
    mAttack3AnimName     = DB->Attack3AnimName;
    mGetHitAnimName      = DB->GetHitAnimName;
    mDeathAnimName       = DB->DeathAnimName;
}
