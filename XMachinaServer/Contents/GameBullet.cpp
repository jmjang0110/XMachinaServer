#include "pch.h"
#include "GameObject.h"
#include "GameBullet.h"
#include "GameMonster.h"
#include "GamePlayer.h"
#include "Collider.h"
#include "CollisionManager.h"


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
	FBProtocol::WEAPON_TYPE weaponType = GetWeaponType();
	BulletUpdate(weaponType);

	// Limit Life Time 
	float deltaTime = GetDeltaTime();
	mCurrLifeTime += deltaTime;
	if (mCurrLifeTime >= mMaxLifeTime) {
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

void GameBullet::BulletUpdate(FBProtocol::WEAPON_TYPE weaponType)
{
	switch (weaponType)
	{
	case FBProtocol::WEAPON_TYPE_H_LOOK:
	case FBProtocol::WEAPON_TYPE_DBMS:
	case FBProtocol::WEAPON_TYPE_STUART:
	case FBProtocol::WEAPON_TYPE_DESCRIPTOR:
	case FBProtocol::WEAPON_TYPE_T_12:
	case FBProtocol::WEAPON_TYPE_PIPELINE:
	case FBProtocol::WEAPON_TYPE_DIRECT_DRAIN:
	case FBProtocol::WEAPON_TYPE_SKYLINE:
		CheckCollision_WithHitMonsterID();
		break;
	case FBProtocol::WEAPON_TYPE_BURNOUT://
		CheckCollision_WithPlayerViewList();
		break;

	default: {

	}
		break;
	}

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

void GameBullet::CheckCollision_WithHitMonsterID()
{
	ViewList player_VL = mOwnerPlayer->S_GetViewList(); // Lock : Player View List 
	const auto& iter = player_VL.VL_Monsters.find(static_cast<UINT32>(mHitMonsterID));
	
	if (iter != player_VL.VL_Monsters.end()) {
		if (iter->second->S_GetObjectState() == Script_Stat::ObjectState::Dead) {
			DeActivate();
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

