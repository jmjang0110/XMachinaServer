#include "pch.h"
#include "CollisionManager.h"
#include "Collider.h"

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

bool CollisionManager::CollideCheck_Sphere(ColliderSnapShot& A, ColliderSnapShot& B)
{
	if (A.BS.Intersects(B.BS)) {
		return true;
	}

	return false;
}

bool CollisionManager::CollideCheck_Box(ColliderSnapShot& A, ColliderSnapShot& B)
{
	/* Box - Box */
	for (auto& a : A.BoundingBoxList) {
		for (auto& b : B.BoundingBoxList) {
			if (a.Intersects(b))
				return true;
		}
	}

	return false;
}

bool CollisionManager::CollideCheck_Sphere_Box(ColliderSnapShot& A_sphere, ColliderSnapShot& B_box)
{
	/* Box - Sphere */
	for (auto& b : B_box.BoundingBoxList) {
		if (b.Intersects(A_sphere.BS))
			return true;
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

bool CollisionManager::CollideCheck(Vec3& A, Vec3& B, float dist)
{
	if ((A - B).Length() <= dist)
		return true;
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

bool CollisionManager::CollideCheckRay(const ColliderSnapShot& A, Ray& R)
{
	float dist = 100.0f;

	if (!A.BS.Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist)) {
		return true;
	}

	for (int i = 0; i < A.BoundingBoxList.size(); ++i) {
		if (A.BoundingBoxList[i].Intersects(_VECTOR(R.Position), XMVector3Normalize(_VECTOR(R.Direction)), dist)) {
			return true;
		}
	}

	return false;
}
