#include "pch.h"
#include "GameBullet.h"

GameBullet::GameBullet()
	: GameObject()
{
}

GameBullet::GameBullet(UINT32 sessionID)
	: GameObject(sessionID)
{
}

GameBullet::GameBullet(UINT32 sessionID, SPtr<GamePlayer> owner)
	: GameObject(sessionID)
{
	mOwnerPlayer = owner;

}

GameBullet::~GameBullet()
{
}

void GameBullet::Update()
{
	FBProtocol::WEAPON_TYPE weaponType = GetWeaponType();


	// Ray Check Or Simultaion 
	switch (weaponType)
	{
	case FBProtocol::WEAPON_TYPE_H_LOOK:{

	}
		break;
	case FBProtocol::WEAPON_TYPE_DBMS: {

	}
		break;
	case FBProtocol::WEAPON_TYPE_STUART:{

		}
		break;
	case FBProtocol::WEAPON_TYPE_DESCRIPTOR:{

		}
		break;
	case FBProtocol::WEAPON_TYPE_T_12:{

		}
		break;
	case FBProtocol::WEAPON_TYPE_PIPELINE:{

		}
		break;
	case FBProtocol::WEAPON_TYPE_BURNOUT:{

		}
		break;
	case FBProtocol::WEAPON_TYPE_DIRECT_DRAIN:{

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

	if (mInfo.BulletOwner == nullptr)
	{
		mInfo.BulletOwner = std::dynamic_pointer_cast<GameBullet>(shared_from_this());
	}
	if (mActivate_Ref.load() == 1) {
		GameObject::RegisterUpdate();
	}
}

void GameBullet::DeActivate()
{
	GameObject::DeActivate();

	mActivate_Ref.fetch_sub(1);
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

