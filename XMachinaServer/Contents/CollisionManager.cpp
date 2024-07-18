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
	/* Sphere - Sphere Collide Check */
	for (auto& a : A.BoundingSphereList) {
		for (auto& b : B.BoundingSphereList) {
			if (a.Intersects(b))
				return true;
		}
	}

	/* Box - Sphere */
	for (auto& a : A.BoundingBoxList) {
		for (auto& b : B.BoundingSphereList) {
			if (a.Intersects(b))
				return true;
		}
	}

	/* Sphere - Box */
	for (auto& a : A.BoundingSphereList) {
		for (auto& b : B.BoundingBoxList) {
			if (a.Intersects(b))
				return true;
		}
	}

	/* Box - Box */
	for (auto& a : A.BoundingBoxList) {
		for (auto& b : B.BoundingBoxList) {
			if (a.Intersects(b))
				return true;
		}
	}

	return false;
}
