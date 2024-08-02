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

bool CollisionManager::CollideCheck(ColliderSnapShot& A, Ray& R, float dist)
{

	for (int i = 0; i < A.BoundingBoxList.size(); ++i) {
		bool IsCollide = A.BoundingBoxList[i].Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist);
		if (IsCollide)
			return true;
	}

	for (int i = 0; i < A.BoundingSphereList.size(); ++i) {
		bool IsCollide = A.BoundingSphereList[i].Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist);
		if (IsCollide)
			return true;
	}

	return false;
}

float CollisionManager::CollideCheckRay_MinimumDist(const ColliderSnapShot& A, Ray& R)
{
	float minDist = 999.f;
	float dist    = 100;

	for (int i = 0; i < A.BoundingBoxList.size(); ++i) {
		bool IsCollide = A.BoundingBoxList[i].Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist);
		if (IsCollide) {
			minDist = min(minDist, dist);
		}
	}

	for (int i = 0; i < A.BoundingSphereList.size(); ++i) {
		bool IsCollide = A.BoundingSphereList[i].Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist);
		if (IsCollide) {
			minDist = min(minDist, dist);
		}
	}

	return minDist;
}
