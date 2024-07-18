#pragma once
#include "Gameinfo.h"
#include "Collider.h"

#define COLLISION_MGR CollisionManager::GetInst()

class CollisionManager
{
	DECLARE_SINGLETON(CollisionManager);

private:
	using CollidePair  = std::pair<GameObjectInfo::Type, GameObjectInfo::Type>;



public:
	CollisionManager();
	~CollisionManager();


public:
	bool CollideCheck(ColliderSnapShot A, ColliderSnapShot B);
};
 
