#pragma once

#include "Component.h"

#pragma region ClassForwardDecl
class BoxCollider;
class SphereCollider;
class ObjectCollider;
#pragma endregion

class Collider : public Component
{

private:

public:
	enum class Type { None, Box, Sphere };

	virtual  Type	GetType() const abstract;
	virtual Vec3	GetCenter() const abstract;

	BoxCollider*	GetBoxCollider();
	SphereCollider* GetSphereCollider();

	Collider();
	Collider(SPtr<GameObject> owner, ComponentInfo::Type Type);
	~Collider();

public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;


public:
	virtual void OnEnable() override;

	virtual bool Intersects(SPtr<Collider> other) const abstract;
	virtual bool Intersects(const BoundingBox& bb) const abstract;
	virtual bool Intersects(const BoundingOrientedBox& obb) const abstract;
	virtual bool Intersects(const BoundingSphere& bs) const abstract;
	virtual bool Intersects(const Ray& ray, float& dist) const abstract;

protected:
	virtual void UpdateTransform() abstract;

};


// basic cuboid-shaped collision primitive.
class BoxCollider : public Collider {

public:
	BoxCollider();
	BoxCollider(SPtr<GameObject> owner, ComponentInfo::Type Type);
	~BoxCollider();
public:
	MyBoundingOrientedBox mBox{};

public:
	BoxCollider& operator=(const BoxCollider& other);

public:
	virtual Type GetType() const { return Type::Box; }
	virtual Vec3 GetCenter() const { return mBox.Center; }

public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;

	virtual bool Intersects(SPtr<Collider> other) const override;
	virtual bool Intersects(const BoundingBox& bb) const override;
	virtual bool Intersects(const BoundingOrientedBox& obb) const override;
	virtual bool Intersects(const BoundingSphere& bs) const override;
	virtual bool Intersects(const Ray& ray, float& dist) const override;

protected:
	virtual void UpdateTransform() override;

};



// basic sphere-shaped collision primitive.
class SphereCollider : public Collider {

public:
	SphereCollider();
	SphereCollider(SPtr<GameObject> owner, ComponentInfo::Type Type);
	~SphereCollider();

public:
	MyBoundingSphere mBS{};

public:
	SphereCollider& operator=(const SphereCollider& other);

public:
	virtual Type GetType() const { return Type::Sphere; }
	virtual Vec3 GetCenter() const { return mBS.Center; }

public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;

	virtual bool Intersects(SPtr<Collider> other) const override;
	virtual bool Intersects(const BoundingBox& bb) const override;
	virtual bool Intersects(const BoundingOrientedBox& obb) const override;
	virtual bool Intersects(const BoundingSphere& bs) const override;
	virtual bool Intersects(const Ray& ray, float& dist) const override;

protected:
	virtual void UpdateTransform() override;

};

// for collision check
class ObjectCollider : public Component {

	template <typename T>
	static constexpr bool is_valid_collider_type = (std::is_same<T, BoundingBox>::value
		|| std::is_same<T, BoundingOrientedBox>::value
		|| std::is_same<T, MyBoundingOrientedBox>::value
		|| std::is_same<T, BoundingSphere>::value
		|| std::is_same<T, MyBoundingSphere>::value);

private:
	sptr<SphereCollider>		mSphereCollider{};	// (��ü ��ü�� ���δ�)SphereCollider�� �ݵ�� �־�� �ϸ� �ϳ��� �����ؾ� �Ѵ�.
	std::vector<sptr<Collider>>	mColliders{};		// ��� colliders

public:
	ObjectCollider();
	ObjectCollider(SPtr<GameObject> owner, ComponentInfo::Type Type);
	~ObjectCollider();

public:
	const MyBoundingSphere& GetBS() const { return mSphereCollider->mBS; }
	const std::vector<sptr<Collider>>& GetColliders() const { return mColliders; }

public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;

public:

	bool Intersects(const ObjectCollider* other) const;
	bool Intersects(const BoundingFrustum& frustum) const { return frustum.Intersects(mSphereCollider->mBS); }
	bool Intersects(rsptr<Collider> collider) const;

	template<class T, typename std::enable_if<is_valid_collider_type<T>>::type* = nullptr>
	bool Intersects(const T& bounding) const
	{
		for (auto& collider : mColliders) {
			if (collider->Intersects(bounding)) {
				return true;
			}
		}

		return false;
	}

	// �� ��ü�� �浹 ���θ� ��ȯ�Ѵ�.
	static bool Intersects(const GameObject& a, const GameObject& b);
};