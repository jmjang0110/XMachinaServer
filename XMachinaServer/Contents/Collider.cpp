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

void Collider::Activate()
{
}

void Collider::DeActivate()
{
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

    UpdateTransform();

    /* Update Snap Shot */
    UpdateColliderSnapShot();
    SwapSnapShotIndex();

    return true;
}

void Collider::OnDestroy()
{
}

ColliderSnapShot Collider::GetSnapShot()
{
    ColliderSnapShot snapShot = mColliderSnapShot[mSnapShotIndex];
    return snapShot;

}

void Collider::SwapSnapShotIndex()
{
    // [SnapShot Index] Swap!!!!
    mSnapShotIndex != mSnapShotIndex;
}

void Collider::UpdateColliderSnapShot()
{
    std::memcpy(mColliderSnapShot[mSnapShotIndex].BoundingBoxList.data(), mBoundingBoxList.data(), sizeof(MyBoundingOrientedBox) * mBoundingBoxList.size());
    std::memcpy(mColliderSnapShot[mSnapShotIndex].BoundingSphereList.data(), mBoundingSphereList.data(), sizeof(MyBoundingSphere) * mBoundingSphereList.size());

}

void Collider::UpdateTransform()
{

   const Matrix& worldTransform =  GetOwner()->GetTransform()->GetWorldTransform();

   for (auto& iter : mBoundingBoxList) {
       iter.Transform(worldTransform);
   }
   for (auto& iter : mBoundingSphereList) {
       iter.Transform(worldTransform);
   }

}

void Collider::OnEnable()
{
    Component::OnEnable();


}

void Collider::OnDisable()
{
}



//bool ObjectCollider::Intersects(const GameObject& a, const GameObject& b)
//{
//
//    const auto& colliderA = a.GetCollider();
//    const auto& colliderB = b.GetCollider();
//
//    // 반드시 두 객체 모두 ObjectCollider를 가지고 있어야 한다.
//    if (!colliderA || !colliderB) {
//        return false;
//    }
//
//    // 둘 중 하나라도 비활성 상태라면 검사하지 않는다.
//    if (!colliderA->IsActive() || !colliderB->IsActive()) {
//        return false;
//    }
//
//    return colliderA->Intersects(colliderB);
//}

