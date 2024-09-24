#include "pch.h"
#include "Script_SpiderMine.h"
#include "GameObject.h"
#include "GameRoom.h"
#include "SectorController.h"

#include "Component.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "ViewList.h"

#include "Script_Weapon.h"
#include "Script_Player.h"
#include "Script_Enemy.h"



Script_SpiderMine::Script_SpiderMine()
	: Script_ExplosiveBullet()

{
	mMaxLifeTime   = 10.f;
	mSpeed         = 7.5f;
	mMass          = 0.6f;
	mDrag          = 3.f;
	mRotationSpeed = 90.f;
	mUpSpeed       = 10.f;
	mPlantY        = 0.5f;

	mDamage = 200.f;
}

Script_SpiderMine::Script_SpiderMine(SPtr<GameObject> owner)
	: Script_ExplosiveBullet(owner)
{
	mMaxLifeTime   = 10.f;
	mSpeed         = 7.5f;
	mMass          = 0.6f;
	mDrag          = 3.f;
	mRotationSpeed = 90.f;
	mUpSpeed       = 10.f;
	mPlantY        = 0.5f;

	mDamage = 200.f;

}

Script_SpiderMine::~Script_SpiderMine()
{
}

SPtr<Component> Script_SpiderMine::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_AdvancedCombatDroid_5
    auto clonedScript = std::dynamic_pointer_cast<Script_SpiderMine>(target);
    if (clonedScript)
    {
		clonedScript->mMass           = this->mMass;
		clonedScript->mDrag           = this->mDrag;
		clonedScript->mRotationSpeed  = this->mRotationSpeed;;
		clonedScript->mRotationX      = this->mRotationX;
		clonedScript->mRotationXSpeed = this->mRotationXSpeed;
		clonedScript->mPlantY         = this->mPlantY;
		clonedScript->mIsPlanted      = this->mIsPlanted;
		clonedScript->mUpSpeed        = this->mUpSpeed;

		Script_ExplosiveBullet::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_SpiderMine" << std::endl;
        return nullptr;
    }
}
void Script_SpiderMine::Clone(SPtr<GameObject> target)
{
	auto clonedScript = target->AddScript<Script_SpiderMine>();
	this->Clone(clonedScript);
	clonedScript->SetOwner(target);
 }

void Script_SpiderMine::Start()
{

}

void Script_SpiderMine::Update()
{
	Explode();

	//float deltatime	      = mOwner->DeltaTime();
	//if (mIsPlanted) {
	//	mOwner->GetTransform()->Rotate(0, mRotationSpeed * deltatime, 0);
	//	return;
	//}

	//Script_Bullet::Update();

	//Vec3 dir = mOwner->GetTransform()->GetLook();
	//LOG_MGR->Cout("Spider minde dirrection : ", dir.x, " , ", dir.y, " , ", dir.z, "\n");


	//Move(deltatime);

}

void Script_SpiderMine::SplashDamage()
{
	// Bullet �ֺ� range �ȿ� �ִ� Monsters -> Splash Damage 
	
	Vec3					center_pos        = mOwner->GetTransform()->GetPosition();
	auto					sector_controller = mOwner->GetOwnerRoom()->GetSectorController();
	std::vector<Coordinate> sectors           = sector_controller->GetCheckSectors(mOwner->GetTransform()->GetPosition(), mSplashRangeRadius + 1.f);
	
	// Enemies in Splash Range On Hit!
	for (Coordinate& sector : sectors) {
		auto enemies = sector_controller->GetEnemies_InRange(sector, center_pos, mSplashRangeRadius);
		for (auto& e :enemies) {
			auto enemy_entity = e->GetScriptEntity<Script_Enemy>();
			enemy_entity->Hit(mDamage);
		}
	}

}

void Script_SpiderMine::Explode()
{
	mSplashRangeRadius = 5.f;
	mDamage = 200.f;

	Script_ExplosiveBullet::Explode();
	SplashDamage();
}

void Script_SpiderMine::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);
	mOwner->Update();
}

void Script_SpiderMine::Move(float deltatime)
{
	mOwner->GetTransform()->MoveForward(mSpeed * deltatime);

	if (mUpSpeed > 0) {
		mOwner->GetTransform()->MoveUp(mUpSpeed * deltatime);
		mUpSpeed -= deltatime * 10.f;
	}
	mOwner->GetTransform()->MoveUp(-Math::kGravity * mMass * deltatime);


	mSpeed -= Math::Sign(mSpeed) * deltatime * mDrag;


	// Collide Check With Buildings <---> Spider Mine Bullet 
	if (!mIsPlanted) {
		auto room = mOwner->GetOwnerRoom();
		if (room->CollideCheckWithNPC(mOwner, ObjectTag::Building, 15.f)) {
			LOG_MGR->Cout("spider mine Collide with buildings!! ********************* \n");
			if (mSpeed > 0) {
				mSpeed *= -1;
			}
		}
	}

	auto bullet_pos = mOwner->GetTransform()->GetPosition();
	if (bullet_pos.y <= mPlantY) {
		Plant();
		return;
	}

	LOG_MGR->Cout("Bullet Pos : ", bullet_pos.x, " ", bullet_pos.y, " ", bullet_pos.z, "\n");
}

void Script_SpiderMine::Plant() 
{
	if (mOwner->GetOwnerRoom()->CollideCheckWithNPC(mOwner, ObjectTag::Enemy, 15.f))
		Explode();

	mIsPlanted = true;
	mOwner->GetTransform()->SetPositionY(mPlantY);

}