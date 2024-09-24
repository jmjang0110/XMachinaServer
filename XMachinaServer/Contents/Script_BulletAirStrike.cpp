#include "pch.h"
#include "Script_BulletAirStrike.h"
#include "GameObject.h"
#include "GameRoom.h"
#include "RoomManager.h"
#include "SectorController.h"
#include "Script_Enemy.h"


Script_BulletAirStrike::Script_BulletAirStrike(SPtr<GameObject> owner) 
    : Script_ExplosiveBullet(owner)
{
	mSplashRangeRadius = 5.f;
	mSpeed = 20.f;
	mDamage            = 20.f;
}

Script_BulletAirStrike::~Script_BulletAirStrike()
{
}

SPtr<Component> Script_BulletAirStrike::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_AdvancedCombatDroid_5
	auto clonedScript = std::dynamic_pointer_cast<Script_BulletAirStrike>(target);
	if (clonedScript)
	{
		Script_Bullet::Clone(clonedScript);
		clonedScript->mExplosionDamage = this->mExplosionDamage;
		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_BulletAirStrike" << std::endl;
		return nullptr;
	}
}

void Script_BulletAirStrike::Update()
{
	if (mIsExploded)
		return;

	Script_Bullet::Update();

	// Update
	float deltatime = mOwner->DeltaTime();
	mOwner->GetTransform()->MoveForward(deltatime * mSpeed);
	Vec3 bullet_pos = mOwner->GetTransform()->GetPosition();

	//LOG_MGR->Cout("[", mOwner->GetID(), "] : ", bullet_pos.x, ", ", bullet_pos.y, ", ", bullet_pos.z, "\n");

	bool IsCollide{};
	// Collide Check
	if (IsCollide = mOwner->GetOwnerRoom()->CollideCheckWithNPC(mOwner, ObjectTag::Enemy, 10.f)){
		//LOG_MGR->Cout("[", mOwner->GetID(), "] : ", "Collide With Enemy!\n");
	}
	else if (IsCollide = mOwner->GetOwnerRoom()->CollideCheckWithNPC(mOwner, ObjectTag::Building, 15.f)) {
		//LOG_MGR->Cout("[", mOwner->GetID(), "] : ", "Collide With Building!\n");
	}
	else if (IsCollide = bullet_pos.y <= 0.f) {
		//LOG_MGR->Cout("[", mOwner->GetID(), "] : ", "Collide With Ground!\n");

	}

	// Explode 
	if (IsCollide)
		Explode();
}

void Script_BulletAirStrike::SplashDamage()
{
	// Bullet 주변 range 안에 있는 Monsters -> Splash Damage 
	Vec3					center_pos = mOwner->GetTransform()->GetPosition();
	auto					sector_controller = mOwner->GetOwnerRoom()->GetSectorController();
	std::vector<Coordinate> sectors = sector_controller->GetCheckSectors(mOwner->GetTransform()->GetPosition(), mSplashRangeRadius + 1.f);

	// Enemies in Splash Range On Hit!
	for (Coordinate& sector : sectors) {
		auto enemies = sector_controller->GetEnemies_InRange(sector, center_pos, mSplashRangeRadius);
		for (auto& e : enemies) {
			auto enemy_entity = e->GetScriptEntity<Script_Enemy>();
			enemy_entity->Hit(mDamage);
		}
	}
}

void Script_BulletAirStrike::Explode()
{
	Script_ExplosiveBullet::Explode();			// Deactivate 
	SplashDamage();		// SplashDamage 
}

void Script_BulletAirStrike::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);
	mOwner->Update();
}
