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
    mBS = other.mBS;

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

void Collider::Clone(SPtr<Component> other) 
{
    Component::Clone(other);

    SPtr<Collider> collider = std::static_pointer_cast<Collider>(other);

    this->mBS = collider->mBS;
    this->mBoundingBoxList.resize(collider->mBoundingBoxList.size());
    std::memcpy(collider->mBoundingBoxList.data(), collider->mBoundingBoxList.data(), collider->mBoundingBoxList.size() * sizeof(MyBoundingOrientedBox));


    mColliderSnapShot[0].BoundingBoxList = mBoundingBoxList;
    mColliderSnapShot[1].BoundingBoxList = mBoundingBoxList;

    this->SetType(ComponentInfo::Type::Collider);
}

bool Collider::LateUpdate()
{
    UpdateTransform();

    /* Update Snap Shot */
    UpdateColliderSnapShot();
    SwapSnapShotIndex();

    return true;
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
    mColliderSnapShot[mSnapShotIndex].BS = mBS;
    mColliderSnapShot[mSnapShotIndex].BoundingBoxList.resize(mBoundingBoxList.size());

    std::memcpy(mColliderSnapShot[mSnapShotIndex].BoundingBoxList.data(), mBoundingBoxList.data(), sizeof(MyBoundingOrientedBox) * mBoundingBoxList.size());

}

void Collider::UpdateTransform()
{
   const Matrix& worldTransform =  GetOwner()->GetTransform()->GetWorldTransform();
   mBS.Transform(worldTransform);

   for (auto& iter : mBoundingBoxList) {
       iter.Transform(worldTransform);
   }
}

void Collider::SetBoundingBoxList(const std::vector<MyBoundingOrientedBox>& boxList)
{
    mBoundingBoxList.resize(boxList.size());
    std::memcpy(mBoundingBoxList.data(), boxList.data(), sizeof(MyBoundingOrientedBox) * boxList.size());

    for (auto& box : mBoundingBoxList) {
        box.Transform(GetOwner()->GetTransform()->GetWorldTransform());
    }

    mColliderSnapShot[0].BoundingBoxList = mBoundingBoxList;
    mColliderSnapShot[1].BoundingBoxList = mBoundingBoxList;

}
