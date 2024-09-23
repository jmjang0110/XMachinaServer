#include "pch.h"
#include "Script_BulletDeus_Phase_1.h"
#include "GameObject.h"
#include "GameRoom.h"
#include "SectorController.h"
#include "Sector.h"
#include "Script_Player.h"


Script_BulletDeus_Phase_1::Script_BulletDeus_Phase_1(SPtr<GameObject> owner)
	: Script_ExplosiveBullet(owner)
{
	mSplashRangeRadius = 3.f;
	mSpeed             = 5.f * 2.f;
	mDamage            = 20.f;
}

Script_BulletDeus_Phase_1::~Script_BulletDeus_Phase_1()
{
}

SPtr<Component> Script_BulletDeus_Phase_1::Clone(SPtr<Component> target)
{
	return SPtr<Component>();
}

void Script_BulletDeus_Phase_1::SplashDamage()
{
	// Bullet 주변 range 안에 있는 Monsters -> Splash Damage 
	Vec3 center_pos = mOwner->GetTransform()->GetPosition();
	auto players = mOwner->GetOwnerRoom()->GetPlayersInRange(center_pos, mSplashRangeRadius);

	for (auto player : players) {
		auto player_entity = player->GetScriptEntity<Script_Player>();
		if (player_entity)
			player_entity->Hit(mDamage);
	}

}

void Script_BulletDeus_Phase_1::Explode()
{
	Script_ExplosiveBullet::Explode();			// Deactivate 
	SplashDamage();		// SplashDamage 
}


void Script_BulletDeus_Phase_1::Update()
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
	if (IsCollide = mOwner->GetOwnerRoom()->CollideCheckWithPlayer(mOwner)) {
		LOG_MGR->Cout("[", mOwner->GetID(), "] : ", "Collide With Player!\n");
	}
	else if (IsCollide = mOwner->GetOwnerRoom()->CollideCheckWithNPC(mOwner, ObjectTag::Building)) {
		LOG_MGR->Cout("[", mOwner->GetID(), "] : ", "Collide With Building!\n");
	}
	else if (IsCollide = bullet_pos.y <= 0.f) {
		LOG_MGR->Cout("[", mOwner->GetID(), "] : ", "Collide With Ground!\n");

	}

	// Explode 
	if (IsCollide)
		Explode();

}
void Script_BulletDeus_Phase_1::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);
	mOwner->Update();
}
