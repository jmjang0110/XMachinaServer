#pragma once

#include "Component.h"
#include "ObjectSnapShot.h"

#pragma region ClassForwardDecl
class BoxCollider;
class SphereCollider;
class ObjectCollider;
class Collider;
#pragma endregion

namespace ColliderInfo
{
	enum class Type{ None, Box, Sphere, End, };
}

/// +-----------------------------------------------------
///					COLLIDER SNAPSHOT 
/// -----------------------------------------------------+
struct ColliderSnapShot : public ObjectSnapShot
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
	Collider(SPtr<GameObject> owner, ComponentInfo::Type Type);
	~Collider();

public:
	virtual void Clone(SPtr<Component> CopyT) ;
	virtual bool LateUpdate();

public:
	ColliderSnapShot GetSnapShot(); /* Read Only */
	void SwapSnapShotIndex();
	void UpdateColliderSnapShot();
	void UpdateTransform();

	void SetBS(const MyBoundingSphere& bs) { mBS = bs; }
	void SetBoundingBoxList(const std::vector<MyBoundingOrientedBox>& boxList);

	const MyBoundingSphere& GetBS() const { return mBS; }
	std::vector<MyBoundingOrientedBox>& GetBoundingBoxList() { return mBoundingBoxList; }

	ColliderSnapShot GetColliderSnapShot() { return mColliderSnapShot[mSnapShotIndex]; }
};
