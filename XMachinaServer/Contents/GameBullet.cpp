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
	else
	{

	}
}

void GameBullet::BulletUpdate(FBProtocol::WEAPON_TYPE weaponType)
{
	CheckCollision_WithPlayerViewList();

}

void GameBullet::CheckCollision_WithPlayerViewList()
{
	ViewList player_VL = mOwnerPlayer->GetSNS_ViewList(); // Lock : Player View List 

	for (auto& iter : player_VL.VL_Monsters) {

		if(iter.second->GetSNS_IsDead())
			continue;

		ColliderSnapShot A = iter.second->GetCollider()->GetColliderSnapShot();

		Ray R;
		R.Direction = mOnShootDir;
		R.Position  = GetTransform()->GetPosition();
		bool IsCollide = COLLISION_MGR->CollideCheck(A, R, 0.f);
		if (IsCollide) {
			iter.second->OnHit();
			DeActivate();
			break;
		}
	}

}

