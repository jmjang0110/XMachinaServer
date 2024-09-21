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
}

Script_SpiderMine::Script_SpiderMine(SPtr<GameObject> owner)
	: Script_ExplosiveBullet(owner)
{
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
	Script_Bullet::Update();

	float deltatime	      = mOwner->DeltaTime();
	auto  weaponentity    = mOwnerWeapon->GetScriptEntity<Script_Weapon>();
	auto  player          = weaponentity->GetOwnerPlayer();
	bool  IsExploded      = false;

	if (!player)
		return;

	auto	 player_entity   = player->GetScriptEntity<Script_Player>();
	ViewList player_VL       = player_entity->S_GetViewList();

	mOwner->GetTransform()->MoveForward(mSpeed * deltatime);

	if (mUpSpeed > 0) {
		mOwner->GetTransform()->Translate(Vector3::Up, mUpSpeed * deltatime);
		mUpSpeed -= deltatime * 10.f;
	}
	
	mOwner->GetTransform()->Translate(Vector3::Up ,-Math::kGravity * mMass * deltatime); // translate 

	if (!mIsPlanted) {
		auto room = mOwner->GetOwnerRoom();
		if (room->CollideCheckWithNPC(mOwner, ObjectTag::Building)) {
			if (mSpeed > 0) {
				mSpeed *= -1; // 벽에 부딪치면 퉁긴다.
			}
		}
	}

	mSpeed -= Math::Sign(mSpeed) * deltatime * mDrag;
	auto bullet_pos	= mOwner->GetTransform()->GetPosition();
	if (bullet_pos.y <= mPlantY) {
		/* Plant */
		mIsPlanted = true;
		mOwner->GetTransform()->SetPositionY(mPlantY);
	}
}

void Script_SpiderMine::SplashDamage()
{
	// Bullet 주변 range 안에 있는 Monsters -> Splash Damage 
	
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
	Script_ExplosiveBullet::Explode();
	SplashDamage();
}

void Script_SpiderMine::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);
	mOwner->Update();
}
