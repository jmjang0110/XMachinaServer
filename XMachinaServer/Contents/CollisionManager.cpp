#include "pch.h"
#include "CollisionManager.h"


DEFINE_SINGLETON(CollisionManager);

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::CollideCheck(ColliderSnapShot& A, ColliderSnapShot& B)
{
	if (A.BS.Intersects(B.BS)) {
		return true;
	}

	/* Box - Sphere */
	for (auto& a : A.BoundingBoxList) {
		if (a.Intersects(B.BS))
			return true;
	}

	/* Sphere - Box */
	for (auto& b : B.BoundingBoxList) {
		if (b.Intersects(A.BS))
			return true;
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

bool CollisionManager::CollideCheck(ColliderSnapShot& A, Ray& R, float dist)
{
	if (!A.BS.Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist)) {
		return false;
	}

	if (A.BoundingBoxList.empty()) {
		return true;
	}

	for (int i = 0; i < A.BoundingBoxList.size(); ++i) {
		if (A.BoundingBoxList[i].Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist)) {
			return true;
		}
	}

	return false;
}

float CollisionManager::CollideCheckRay_MinimumDist(const ColliderSnapShot& A, Ray& R)
{
	float minDist = 999.f;
	float dist    = 100;

	if (!A.BS.Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist)) {
		return minDist;
	}

	for (int i = 0; i < A.BoundingBoxList.size(); ++i) {
		if (A.BoundingBoxList[i].Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist)) {
			minDist = min(minDist, dist);
		}
	}

	return minDist;
}
