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


struct ColliderSnapShot : public ObjectSnapShot
{
	std::vector<MyBoundingSphere>		BoundingSphereList{};
	std::vector<MyBoundingOrientedBox>	BoundingBoxList{};
};

class Collider : public Component
{
private:
	std::atomic_bool mSnapShotIndex = 0;
	ColliderSnapShot mColliderSnapShot[2]{};

	std::vector<MyBoundingSphere>		mBoundingSphereList{};
	std::vector<MyBoundingOrientedBox>	mBoundingBoxList{};

public:
	Collider();
	Collider(SPtr<GameObject> owner, ComponentInfo::Type Type);
	~Collider();

public:
	virtual void Activate();
	virtual void DeActivate();

	virtual void OnEnable();
	virtual void OnDisable();

public:
	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();

public:
	ColliderSnapShot GetSnapShot(); /* Read Only */
	void SwapSnapShotIndex();
	void UpdateColliderSnapShot();
	void UpdateTransform();


};


//CollisionMgr
//{
//	static bool CollideCheck(ColliderSnapShot A,  ColliderSnapShot B)
//	{
//
//	}
//
//	MyBoundingSphere();
//	ASPDf
//
//		Instetsrecets
//
//
//		reutrn;
//
//	}
//
//}
//
//CollisionMgr->( Monster , Player )
//{
//	for (int i = 0; i < Player.hcils.size()l + _i) {
//		bool check = CollisMgr->Collidechl(Palyer.Child / Monster);
//		
//		monster->Oncollosion();
//		Player.child->OnCollision();
//
//	}
//}
//
//bool CollisMgr->CheckCollision(a, b)
//{
//	a->ObjectCollider->CheckCollision(b->objectCollider)
//
//		a.Type;
//	b.Type;
//	a.Type == sphere && b.Type == Box
//	{
//		return a.BoundingBox < --->Intersects b.BoundingBox;
//	}
//}
//class ObjectCollider
//{
//
//	Collider Ä³½Ì¿ë
//		std::vector<SPtr<Collider>>
//
//	bool CheckCollision(ObjectCllider other)
//	{
//	}
//
//}
//
//class Collider : public Component
//{
//	ObjectCollider* col = nullptr;;
//
//
//private:
//	std::vector<MyBoundingSphere>		Sphere;
//	std::vector<MyBoundingOrientedBox>  boxs;
//
//	GetSnapShot()
//	{
//		std::vector<MyBoundingSphere>		Sphere;
//		std::vector<MyBoundingOrientedBox>  boxs;
//	}reutrn;
//
//public:
//	Update(Vec3 Position) {
//		Matrix mat = Piosition;
//		Box 
//			Shptetr
//	}
//
//	bool CollisionCheck(MyBoundingSphere);
//	bool CollisionCheck(MyBoundingOrientedBox);
//};