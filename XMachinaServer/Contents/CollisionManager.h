#pragma once
#include "Gameinfo.h"
#include "Collider.h"

#define COLLISION_MGR CollisionManager::GetInst()

class CollisionManager
{
	DECLARE_SINGLETON(CollisionManager);

public:
	CollisionManager();
	~CollisionManager();

public:
	bool CollideCheck(ColliderSnapShot A, ColliderSnapShot B);

};
 
