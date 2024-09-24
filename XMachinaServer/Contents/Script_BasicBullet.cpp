#include "pch.h"
#include "Script_BasicBullet.h"
#include "GameObject.h"
#include "GameRoom.h"
#include "Transform.h"



Script_BasicBullet::Script_BasicBullet()
    : Script_Bullet()
{
	mBulletType = BulletType::Basic;
}

Script_BasicBullet::Script_BasicBullet(SPtr<GameObject> owner)
    : Script_Bullet(owner)
{
	mBulletType = BulletType::Basic;
}

Script_BasicBullet::~Script_BasicBullet()
{
}

SPtr<Component> Script_BasicBullet::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_AdvancedCombatDroid_5
	auto clonedScript = std::dynamic_pointer_cast<Script_BasicBullet>(target);
	if (clonedScript)
	{
		Script_Bullet::Clone(clonedScript);
		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_BasicBullet" << std::endl;
		return nullptr;
	}
}

void Script_BasicBullet::Clone(SPtr<GameObject> target)
{
	auto clonedScript = target->AddScript<Script_BasicBullet>();
	this->Clone(clonedScript);
	clonedScript->SetOwner(target);
}

void Script_BasicBullet::Update()
{
	Script_Bullet::Update();

	float deltatime = mOwner->DeltaTime();
	mOwner->GetTransform()->MoveForward(deltatime * mSpeed);
	
	bool IsCollide = false;
	IsCollide = mOwner->GetOwnerRoom()->CollideCheckWithNPC(mOwner, ObjectTag::Enemy, 10.f);
	IsCollide = mOwner->GetOwnerRoom()->CollideCheckWithNPC(mOwner, ObjectTag::Building, 15.f);
	
	if (IsCollide)
		Explode();
}

void Script_BasicBullet::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);
	mOwner->Update();
}
