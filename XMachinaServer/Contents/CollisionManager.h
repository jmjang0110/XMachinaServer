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
	static float CollideCheckRay_MinimumDist(const ColliderSnapShot& A, Ray& R);
	
public:
	static bool CollideCheck(ColliderSnapShot& A, ColliderSnapShot& B);
	static bool CollideCheck(ColliderSnapShot& A, Ray& R, float dist);
	static bool CollideCheck(Vec3& A, Vec3& B, float dist);
	
};
 
