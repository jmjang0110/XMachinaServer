#include "pch.h"
#include "GameObject.h"
#include "GameBullet.h"
#include "GameMonster.h"
#include "GamePlayer.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Transform.h"
#include "SectorController.h"
#include "Sector.h"
#include "PlayerController.h"
#include "GameRoom.h"
#include "Collider.h"

GameBullet::GameBullet()
	: GameObject()
{
	SetType(GameObjectInfo::Type::Bullet);

}

GameBullet::GameBullet(UINT32 sessionID)
	: GameObject(sessionID)
{
	SetType(GameObjectInfo::Type::Bullet);

}

GameBullet::GameBullet(UINT32 sessionID, SPtr<GamePlayer> owner)
	: GameObject(sessionID)
{
	mOwnerPlayer      = owner;
	mInfo.PlayerOwner = owner;
	SetType(GameObjectInfo::Type::Bullet);


}

GameBullet::~GameBullet()
{
}

void GameBullet::Update()
{
	GameObject::Update();

	// Update By Weapon Type 
	FBProtocol::ITEM_TYPE weaponType = GetWeaponType();
	BulletUpdate(weaponType);

	if (mCurrMoveDist >= mMaxMoveDist) {
		if(mActivate_Ref.load() != 0)
			DeActivate();
	}

	mCurrLifeTime += GetDeltaTime();
	if (mCurrLifeTime >= mMaxLifeTime) {
		if (mActivate_Ref.load() != 0)
			DeActivate();
	}

}

void GameBullet::Activate()
{
	GameObject::Activate();

	mActivate_Ref.fetch_add(1);

	if (mActivate_Ref.load() == 1) {
		GameObject::RegisterUpdate();
	}
}

void GameBullet::DeActivate()
{
	mActivate_Ref.fetch_sub(1);

	if (mActivate_Ref.load() == 0) {
		int bulletIndex = GetID();
		mInfo.PlayerOwner->Push_PossibleBulletIndex(bulletIndex);
		GameObject::DeActivate();
	}
}

void GameBullet::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);

	Update();

	if (GetActivate_RefCnt() > 0)
		GameObject::RegisterUpdate();
}


void GameBullet::BulletInit(FBProtocol::ITEM_TYPE weaponType)
{
	switch (weaponType)
	{
	case FBProtocol::ITEM_TYPE_WEAPON_H_LOOK:
	case FBProtocol::ITEM_TYPE_WEAPON_DBMS:
	case FBProtocol::ITEM_TYPE_WEAPON_STUART:
	case FBProtocol::ITEM_TYPE_WEAPON_DESCRIPTOR:
	case FBProtocol::ITEM_TYPE_WEAPON_T_12:
	case FBProtocol::ITEM_TYPE_WEAPON_PIPELINE: {
		mMaxLifeTime = 2.f; // 1분 동안 충돌체크 

	}
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_MINE_LAUNCHER: {
		mSpeed          = 7.5f;
		mUpSpeed        = 10.f;
		mMass           = 0.6f;
		mDrag           = 3.f;
		mRotationSpeed  = 90.f;
		mPlantY         = 0.5f;

		mMaxLifeTime	= 60.f; // 1분 동안 충돌체크 
		//GetTransform()->SetLook();
		//GetTransform()->Rotate(mRotationX, 0, 0);

	}
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_BURNOUT: 
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_DIRECT_DRAIN:
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_SKYLINE:
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_AIR_STRIKE:
		break;
	default:
		break;
	}
}

void GameBullet::BulletUpdate(FBProtocol::ITEM_TYPE weaponType)
{
	switch (weaponType)
	{
	case FBProtocol::ITEM_TYPE_WEAPON_H_LOOK:
	case FBProtocol::ITEM_TYPE_WEAPON_DBMS:
	case FBProtocol::ITEM_TYPE_WEAPON_STUART:
	case FBProtocol::ITEM_TYPE_WEAPON_DESCRIPTOR:
	case FBProtocol::ITEM_TYPE_WEAPON_T_12:
	case FBProtocol::ITEM_TYPE_WEAPON_PIPELINE:
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_MINE_LAUNCHER:
		BulletUpdate_SpiderMine();
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_SKYLINE:
		CheckCollision_WithHitMonsterID_Ray();
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_BURNOUT:// Missile
		CheckCollision_WithPlayerViewList();
		break;
	case FBProtocol::ITEM_TYPE_WEAPON_AIR_STRIKE:
		BulletUpdate_Air_Strike();
		break;
	default: {

	}
		break;
	}

}



void GameBullet::BulletUpdate_Air_Strike()
{
	// Bullet Move ! 
	float deltaTime = GetDeltaTime();
	mCurrLifeTime += deltaTime;
	mCurrMoveDist = deltaTime * mSpeed;
	GetTransform()->MoveForward(mCurrMoveDist);

	CollideCheck_SplashDamage(10, 0.f, true);


}

void GameBullet::BulletUpdate_SpiderMine()
{
	ViewList player_VL   = mOwnerPlayer->S_GetViewList(); // Lock : Player View List 

	int		HitID        = -1;
	Vec3	bulletPos    = GetTransform()->GetPosition();
	float	splashDamage = 3.f;
	float	deltatime	 = GetDeltaTime();

	if (GetTransform()->GetPosition().y <= mPlantY) {
		CollideCheck_SplashDamage(10, 0.5f, false);
		return;
	}

	float dist = mSpeed * deltatime;
	GetTransform()->MoveForward(dist);
	if (mUpSpeed > 0) {
		GetTransform()->Translate(Vector3::Up, mUpSpeed * deltatime);
		mUpSpeed -= deltatime * 10.f;
	}
	GetTransform()->Translate(Vector3::Up ,-Math::kGravity * mMass * deltatime); // translate 
	if (!mIsPlanted) {
		if (CheckCollision_WithBuildings()) {
			if (mSpeed > 0) {
				mSpeed *= -1; // 벽에 부딪치면 퉁긴다.
			}
		}
	}

	mSpeed -= Math::Sign(mSpeed) * deltatime * mDrag;
	auto pos		= GetTransform()->GetPosition();
	if (pos.y <= mPlantY) {
		Plant();
	}

	LOG_MGR->Cout(GetID(), " MINE TRANS : ", pos.x, " ", pos.y, " ", pos.z, "\n");

}

bool GameBullet::CollideCheck_SplashDamage(float onHitDamage, float min_y, bool underGroundIsHit/* 바닥에 닿으면 바로 터지는가 안터지는가 */)
{
	bool Result = false;
	// Collide Check /  ( Splash Damage )
	ViewList player_VL = mOwnerPlayer->S_GetViewList(); // Lock : Player View List 

	int		HitID        = -1;
	bool	IsHit        = false;
	Vec3	bulletPos    = GetTransform()->GetPosition();
	float	splashDamage = 4.f;

	// 바로 터진다. 
	if (bulletPos.y <= min_y) IsHit = underGroundIsHit;

	if (IsHit == true) {
		// Check Splash Damage
		for (auto& iter : player_VL.VL_Monsters) {
			if (iter.second->S_GetObjectState() == Script_Stat::ObjectState::Dead) {
				continue;
			}

			Vec3 monsterPos = iter.second->GetTransform()->GetSnapShot().GetPosition();
			bool IsCollide = COLLISION_MGR->CollideCheck(bulletPos, monsterPos, splashDamage);
			if (IsCollide) {
				iter.second->OnHit(static_cast<int>(onHitDamage));

				LOG_MGR->Cout(iter.second->GetID(), "AIR_STRIKE HIT! ( Splash )\n");
				IsHit = true;
				Result = true;
			}
		}

		DeActivate();
		return Result;
	}

	for (auto& iter : player_VL.VL_Monsters) {

		if (iter.second->S_GetObjectState() == Script_Stat::ObjectState::Dead) {
			continue;
		}

		Vec3	monsterPos = iter.second->GetTransform()->GetSnapShot().GetPosition();

		/* Monster(View List) <-- Collide Check --> Bullet */
		bool IsCollide = COLLISION_MGR->CollideCheck(bulletPos, monsterPos, 0.f);
		if (IsCollide) {
			iter.second->OnHit(static_cast<int>(onHitDamage));
			DeActivate();
			LOG_MGR->Cout(iter.second->GetID(), "AIR_STRIKE HIT!\n");
			IsHit = true;
			HitID = iter.second->GetID();
			Result = true;
			break;
		}
	}

	if (IsHit == true) {
		// Check Splash Damage
		for (auto& iter : player_VL.VL_Monsters) {
			if (iter.second->S_GetObjectState() == Script_Stat::ObjectState::Dead ||
				iter.second->GetID() == HitID) {
				continue;
			}

			Vec3	monsterPos = iter.second->GetTransform()->GetSnapShot().GetPosition();
			bool IsCollide = COLLISION_MGR->CollideCheck(bulletPos, monsterPos, splashDamage);
			if (IsCollide) {
				iter.second->OnHit(static_cast<int>(onHitDamage));
				Result = true;
				LOG_MGR->Cout(iter.second->GetID(), " monID -> AIR_STRIKE HIT! ( Splash )\n");
			}
		}
	}

	return Result;
}

void GameBullet::CheckCollision_WithPlayerViewList()
{
	ViewList player_VL = mOwnerPlayer->S_GetViewList(); // Lock : Player View List 

	for (auto& iter : player_VL.VL_Monsters) {

		if (iter.second->S_GetObjectState() == Script_Stat::ObjectState::Dead) {
			continue;
		}

		ColliderSnapShot A = iter.second->GetCollider()->GetColliderSnapShot(); // Monster Collider SnapShot
		Ray				 R = {};	
		R.Direction        = mOnShootDir;					// Bullet Move Dir  
		R.Position         = GetTransform()->GetPosition(); // Bullet Curr Pos 
		
		/* Monster(View List) <-- Collide Check --> Bullet */
		bool IsCollide = COLLISION_MGR->CollideCheck(A, R, 0.f);
		if (IsCollide) {
			iter.second->OnHit();
			DeActivate();
			break;
		}
	}

}

void GameBullet::CheckCollision_WithHitMonsterID_Ray()
{
	ViewList player_VL = mOwnerPlayer->S_GetViewList(); // Lock : Player View List 
	const auto& iter = player_VL.VL_Monsters.find(static_cast<UINT32>(mHitMonsterID));
	
	if (iter != player_VL.VL_Monsters.end()) {
		if (iter->second->S_GetObjectState() == Script_Stat::ObjectState::Dead) {
			return;
		}

		Ray		  R = {};
		R.Direction = mOnShootDir;					 // Bullet Move Dir  
		R.Position  = GetTransform()->GetPosition();  // Bullet Curr Pos  
		ColliderSnapShot A = iter->second->GetCollider()->GetColliderSnapShot(); // Monster Collider SnapShot
		
		/* Monster(View List) <-- Collide Check --> Bullet */
		bool IsCollide = COLLISION_MGR->CollideCheck(A, R, 0.f);
		if (IsCollide) {
			iter->second->OnHit();
		}
	}

	DeActivate();

}

bool GameBullet::CheckCollision_WithBuildings()
{
	Vec3 pos = GetTransform()->GetPosition();
	auto sc  = GetOwnerPlayer()->GetOwnerPlayerController()->GetOwnerRoom()->GetSectorController();
	auto col = GetCollider()->GetColliderSnapShot();
	bool IsCollide = sc->CollideCheck_WithBuildings(pos, col);
	return IsCollide;
}

void GameBullet::Plant()
{
	mIsPlanted = true;
	GetTransform()->SetPositionY(mPlantY);
}

