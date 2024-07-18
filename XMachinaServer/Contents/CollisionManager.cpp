#include "pch.h"
#include "CollisionManager.h"


DEFINE_SINGLETON(CollisionManager);

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::CollideCheck(ColliderSnapShot A, ColliderSnapShot B)
{
	return false;
}
