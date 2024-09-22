#include "pch.h"
#include "Script_RayCheckBullet.h"

#include "NPCController.h" 
#include "CollisionManager.h"

#include "GameObject.h"
#include "GameRoom.h"

#include "Transform.h"
#include "Collider.h"

#include "Script_Enemy.h"

Script_RayCheckBullet::Script_RayCheckBullet(SPtr<GameObject> owner)
	: Script_Bullet(owner)
{
	mBulletType = BulletType::RayCheck;
}

Script_RayCheckBullet::~Script_RayCheckBullet()
{
}

SPtr<Component> Script_RayCheckBullet::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_AdvancedCombatDroid_5
	auto clonedScript = std::dynamic_pointer_cast<Script_RayCheckBullet>(target);
	if (clonedScript)
	{
		Script_Bullet::Clone(clonedScript);
		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_RayCheckBullet" << std::endl;
		return nullptr;
	}
}

void Script_RayCheckBullet::Clone(SPtr<GameObject> target)
{
	auto clonedScript = target->AddScript<Script_RayCheckBullet>();
	this->Clone(clonedScript);
	clonedScript->SetOwner(target);
};


void Script_RayCheckBullet::Update()
{
	// Ray Check 
	RayCheck();

	if(!mIsExploded)
		Explode();
}

void Script_RayCheckBullet::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);

	mOwner->Update();
}

void Script_RayCheckBullet::RayCheck()
{
	int raycheckID = mRayCheckTargetID;
	if (raycheckID == -1)
		return;

	bool raycheck{};
	auto enemy = mOwner->GetOwnerRoom()->GetNPCController()->GetMonster(mRayCheckTargetID);
	
	if (enemy) {
		// A : [Enemy]  Collider SnapShot 
		auto lhs = enemy->GetCollider()->GetColliderSnapShot();
		// B : [Bullet] Ray
		Ray rhs{};
		rhs.Direction = mOwner->GetTransform()->GetLook();
		rhs.Position  = mOwner->GetTransform()->GetPosition();

		// Ray Check! 
		raycheck = COLLISION_MGR->CollideCheckRay(lhs, rhs);
		if (raycheck) {
			LOG_MGR->Cout(enemy->GetID(), " Enemy <-- On (O) Hit --> Bullet ", mOwner->GetID(), "\n");
			auto enemy_entity = enemy->GetScriptEntity<Script_Enemy>();
			enemy_entity->Hit(mDamage);

			if (!mIsExploded)
				Explode();
		}
	}
	
	SetRayCheckTargetID(-1);
}
