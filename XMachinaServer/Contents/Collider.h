#pragma once

#include "Component.h"

#pragma region ClassForwardDecl
class BoxCollider;
class SphereCollider;
class ObjectCollider;
class Collider;
class GameObject;
#pragma endregion

namespace ColliderInfo
{
	enum class Type{ None, Box, Sphere, End, };
}

/// +-----------------------------------------------------
///					COLLIDER SNAPSHOT 
/// -----------------------------------------------------+
struct ColliderSnapShot 
{
	MyBoundingSphere BS{};
	std::vector<MyBoundingOrientedBox>	BoundingBoxList{};
};

/// +-----------------------------------------------------
///				    COLLIDER COMPONENT 
/// -----------------------------------------------------+
class Collider : public Component
{
private:
	std::atomic_bool mSnapShotIndex = 0;
	ColliderSnapShot mColliderSnapShot[2]{};

	MyBoundingSphere mBS{};
	std::vector<MyBoundingOrientedBox>	mBoundingBoxList{};

public:
	Collider();
	Collider(const Collider& other);
	Collider(SPtr<GameObject> owner, Component::Type Type);
	~Collider();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target) ;
	virtual void LateUpdate() override;

public:
	ColliderSnapShot GetSnapShot(); /* Read Only */
	void UpdateColliderSnapShot();
	void SyncSnapShot();

public:
	void UpdateTransform();

	void SetBS(const MyBoundingSphere& bs) { mBS = bs; }
	void SetBoundingBoxList(const std::vector<MyBoundingOrientedBox>& boxList);

	const MyBoundingSphere& GetBS() const { return mBS; }
	std::vector<MyBoundingOrientedBox>& GetBoundingBoxList() { return mBoundingBoxList; }

	ColliderSnapShot GetColliderSnapShot() { return mColliderSnapShot[mSnapShotIndex]; }
};
