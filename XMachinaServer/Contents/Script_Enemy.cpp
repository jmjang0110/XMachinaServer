#include "pch.h"
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

}

void Script_Enemy::AttackCallback()
{
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

