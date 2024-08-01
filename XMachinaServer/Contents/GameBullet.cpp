#include "pch.h"
#include "GameObject.h"
#include "GameBullet.h"
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

	float deltaTime = GetDeltaTime();
	mCurrLifeTime += deltaTime;
	if (mCurrLifeTime >= mMaxLifeTime) {
		DeActivate();
	}

	FBProtocol::WEAPON_TYPE weaponType = GetWeaponType();


	// Ray Check Or Simultaion 
	switch (weaponType)
	{
	case FBProtocol::WEAPON_TYPE_H_LOOK:{
		LOG_MGR->Cout("GameBullet : WEAPON_TYPE_H_LOOK \n");
	}
		break;
	case FBProtocol::WEAPON_TYPE_DBMS: {
		LOG_MGR->Cout("GameBullet : WEAPON_TYPE_DBMS \n");

	}
		break;
	case FBProtocol::WEAPON_TYPE_STUART:{
		LOG_MGR->Cout("GameBullet : WEAPON_TYPE_STUART \n");

		}
		break;
	case FBProtocol::WEAPON_TYPE_DESCRIPTOR:{
		LOG_MGR->Cout("GameBullet : WEAPON_TYPE_DESCRIPTOR \n");

		}
		break;
	case FBProtocol::WEAPON_TYPE_T_12:{
		LOG_MGR->Cout("GameBullet : WEAPON_TYPE_T_12 \n");

		}
		break;
	case FBProtocol::WEAPON_TYPE_PIPELINE:{
		LOG_MGR->Cout("GameBullet : WEAPON_TYPE_PIPELINE \n");

		}
		break;
	case FBProtocol::WEAPON_TYPE_BURNOUT:{
		LOG_MGR->Cout("GameBullet : WEAPON_TYPE_BURNOUT \n");

		}
		break;
	case FBProtocol::WEAPON_TYPE_DIRECT_DRAIN:{
		LOG_MGR->Cout("GameBullet : WEAPON_TYPE_DIRECT_DRAIN \n");

		}
		break;
	case FBProtocol::WEAPON_TYPE_SKYLINE: {
		LOG_MGR->Cout("GameBullet : WEAPON_TYPE_SKYLINE \n");
	}
	 break;
	default:
		assert(0);
		break;
	};

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

