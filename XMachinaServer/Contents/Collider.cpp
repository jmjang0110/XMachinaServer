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

Collider::Collider(const Collider& other)
    : Component(other)
{
    // mBoundingSphereList 복사
    mBoundingSphereList.resize(other.mBoundingSphereList.size());
    if (!other.mBoundingSphereList.empty()) {
        memcpy(mBoundingSphereList.data(), other.mBoundingSphereList.data(),
            other.mBoundingSphereList.size() * sizeof(MyBoundingSphere));
    }

    // mBoundingBoxList 복사
    mBoundingBoxList.resize(other.mBoundingBoxList.size());
    if (!other.mBoundingBoxList.empty()) {
        memcpy(mBoundingBoxList.data(), other.mBoundingBoxList.data(),
            other.mBoundingBoxList.size() * sizeof(MyBoundingOrientedBox));
    }
}

Collider::Collider(SPtr<GameObject> owner, ComponentInfo::Type Type)
    : Component(owner, Type, static_cast<UINT32>(Type))
{

}

Collider::~Collider()
{
}

SPtr<Component> Collider::Clone(SPtr<GameObject> copyOwner) const
{
    return SPtr<Component>();
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


    return true;
}

bool Collider::LateUpdate()
{
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
    mSnapShotIndex.store(!mSnapShotIndex);;
}

void Collider::UpdateColliderSnapShot()
{
    auto& myBoundingSphereList = mColliderSnapShot[mSnapShotIndex].BoundingBoxList;
    auto& myBoundingBoxList = mColliderSnapShot[mSnapShotIndex].BoundingBoxList;
    myBoundingSphereList.resize(mBoundingSphereList.size());
    myBoundingBoxList.resize(mBoundingBoxList.size());

    std::memcpy(mColliderSnapShot[mSnapShotIndex].BoundingSphereList.data(), mBoundingSphereList.data(), sizeof(MyBoundingSphere) * mBoundingSphereList.size());
    std::memcpy(mColliderSnapShot[mSnapShotIndex].BoundingBoxList.data(), mBoundingBoxList.data(), sizeof(MyBoundingOrientedBox) * mBoundingBoxList.size());

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

void Collider::SetBoundingSphereList(const std::vector<MyBoundingSphere>& sphereList)
{
    mBoundingSphereList.resize(sphereList.size());
    std::memcpy(mBoundingSphereList.data(), sphereList.data(), sizeof(MyBoundingSphere) * sphereList.size());
}

void Collider::SetBoundingBoxList(const std::vector<MyBoundingOrientedBox>& boxList)
{
    mBoundingBoxList.resize(boxList.size());
    std::memcpy(mBoundingBoxList.data(), boxList.data(), sizeof(MyBoundingOrientedBox) * boxList.size());
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

