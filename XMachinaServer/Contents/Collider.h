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
	std::vector<MyBoundingSphere>		BoundingSphereList{};
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

	std::vector<MyBoundingSphere>		mBoundingSphereList{};
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

	void SetBoundingSphereList(const std::vector<MyBoundingSphere>& sphereList);
	void SetBoundingBoxList(const std::vector<MyBoundingOrientedBox>& boxList);

	std::vector<MyBoundingSphere>& GetBoundingSphereList() { return mBoundingSphereList; }
	std::vector<MyBoundingOrientedBox>& GetBoundingBoxList() { return mBoundingBoxList; }

};
