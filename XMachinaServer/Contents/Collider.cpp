#include "pch.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

/// +----------------------------------------------------------------------
///     COLLIDER 
/// ----------------------------------------------------------------------+
Collider::Collider()
{
}

Collider::Collider(SPtr<GameObject> owner, ComponentInfo::Type Type)
    : Component(owner, Type, static_cast<UINT32>(Type))
{
}

Collider::~Collider()
{
}

BoxCollider* Collider::GetBoxCollider()
{
    if (GetType() != Type::Box) {
        return nullptr;
    }

    return static_cast<BoxCollider*>(this);
}

SphereCollider* Collider::GetSphereCollider()
{
    if (GetType() != Type::Sphere) {
        return nullptr;
    }

    return static_cast<SphereCollider*>(this);
}



bool Collider::WakeUp()
{
    Component::WakeUp();

    return true;
}

bool Collider::Start()
{
    Component::Start();

    return true;
}

bool Collider::Update()
{
    Component::Update();

    return true;
}

void Collider::OnEnable()
{
    Component::OnEnable();

    UpdateTransform();
}


/// +----------------------------------------------------------------------
///     SphereCollider 
/// ----------------------------------------------------------------------+

SphereCollider::SphereCollider()
{
}

SphereCollider::SphereCollider(SPtr<GameObject> owner, ComponentInfo::Type Type)
    : Collider(owner, Type)
{
}

SphereCollider::~SphereCollider()
{
}

SphereCollider& SphereCollider::operator=(const SphereCollider& other)
{
    if (this == &other) {
        return *this;
    }

    mBS = other.mBS;

    return *this;
}

bool SphereCollider::WakeUp()
{
    return true;
}

bool SphereCollider::Start()
{
    return true;
}

bool SphereCollider::Update()
{
    Collider::Update();

    UpdateTransform();

    return true;
}

bool SphereCollider::Intersects(SPtr<Collider> other) const
{
    if (!IsActive()) {
        return false;
    }

    switch (other->GetType()) {
    case Type::Box:
        return mBS.Intersects(reinterpret_cast<BoxCollider*>(other.get())->mBox);
    case Type::Sphere:
        return mBS.Intersects(reinterpret_cast<SphereCollider*>(other.get())->mBS);
    default:
        assert(0);
        break;
    }

    return false;
}

bool SphereCollider::Intersects(const BoundingBox& bb) const
{
    if (!IsActive()) {
        return false;
    }

    return mBS.Intersects(bb);
}

bool SphereCollider::Intersects(const BoundingOrientedBox& obb) const
{
    if (!IsActive()) {
        return false;
    }

    return mBS.Intersects(obb);
}

bool SphereCollider::Intersects(const BoundingSphere& bs) const
{
    if (!IsActive()) {
        return false;
    }

    return mBS.Intersects(bs);
}

bool SphereCollider::Intersects(const Ray& ray, float& dist) const
{
    if (!IsActive()) {
        return false;
    }

    return mBS.Intersects(_VECTOR(ray.Position), _VECTOR(ray.Direction), dist);
}

void SphereCollider::UpdateTransform()
{
    mBS.Transform(GetOwner()->GetTransform()->GetWorldTransform());

}


/// +----------------------------------------------------------------------
///     BoxCollider 
/// ----------------------------------------------------------------------+


BoxCollider::BoxCollider()
    : Collider()
{
}

BoxCollider::BoxCollider(SPtr<GameObject> owner, ComponentInfo::Type Type)
    : Collider(owner, Type)
{
}

BoxCollider::~BoxCollider()
{
}

BoxCollider& BoxCollider::operator=(const BoxCollider& other)
{
    if (this == &other) {
        return *this;
    }

    mBox = other.mBox;

    return *this;
}

bool BoxCollider::WakeUp()
{
    return false;
}

bool BoxCollider::Start()
{
    return false;
}

bool BoxCollider::Update()
{
    Collider::Update();

    UpdateTransform();

    return true;

}

bool BoxCollider::Intersects(SPtr<Collider> other) const
{
    if (!IsActive()) {
        return false;
    }

    switch (other->GetType()) {
    case Type::Box:
        return mBox.Intersects(reinterpret_cast<BoxCollider*>(other.get())->mBox);
    case Type::Sphere:
        return mBox.Intersects(reinterpret_cast<SphereCollider*>(other.get())->mBS);
    default:
        assert(0);
        break;
    }

    return false;
}

bool BoxCollider::Intersects(const BoundingBox& bb) const
{
    if (!IsActive()) {
        return false;
    }

    return mBox.Intersects(bb);
}

bool BoxCollider::Intersects(const BoundingOrientedBox& obb) const
{
    if (!IsActive()) {
        return false;
    }

    return mBox.Intersects(obb);;
}

bool BoxCollider::Intersects(const BoundingSphere& bs) const
{
    if (!IsActive()) {
        return false;
    }

    return mBox.Intersects(bs);
}

bool BoxCollider::Intersects(const Ray& ray, float& dist) const
{
    if (!IsActive()) {
        return false;
    }

    if (Vector3::IsZero(ray.Direction)) {
        return false;
    }
    return mBox.Intersects(_VECTOR(ray.Position), XMVector3Normalize(_VECTOR(ray.Direction)), dist);
}

void BoxCollider::UpdateTransform()
{
    mBox.Transform(GetOwner()->GetTransform()->GetWorldTransform());

}


/// +----------------------------------------------------------------------
///     ObjectCollider 
/// ----------------------------------------------------------------------+


ObjectCollider::ObjectCollider()
{
}

ObjectCollider::ObjectCollider(SPtr<GameObject> owner, ComponentInfo::Type Type)
    : Component(owner, ComponentInfo::Type::ObjectCollider, static_cast<UINT32>(ComponentInfo::Type::ObjectCollider))
{
}

ObjectCollider::~ObjectCollider()
{
}

bool ObjectCollider::WakeUp()
{
    return false;
}

bool ObjectCollider::Start()
{
    return false;
}

bool ObjectCollider::Update()
{
    return false;
}

bool ObjectCollider::Intersects(const ObjectCollider* other) const
{
    return false;
}

bool ObjectCollider::Intersects(rsptr<Collider> collider) const
{
    return false;
}

bool ObjectCollider::Intersects(const GameObject& a, const GameObject& b)
{
    return false;
}

