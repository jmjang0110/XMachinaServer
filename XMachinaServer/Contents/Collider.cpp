#include "pch.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Rigidbody.h"

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

Collider::Collider(SPtr<GameObject> owner, Component::Type Type)
    : Component(owner, Type, static_cast<UINT32>(Type))
{

}

Collider::~Collider()
{
}

SPtr<Component> Collider::Clone(SPtr<Component> target)
{
    // target이 nullptr인 경우, 새로운 Collider 인스턴스를 생성합니다.
    if (!target)
    {
        target = std::make_shared<Collider>(mOwner, mType);
    }

    // 기본 Component 클래스의 Clone 메서드를 호출하여 공통 속성을 복사합니다.
    Component::Clone(target);

    // Collider 고유의 속성을 복사합니다.
    SPtr<Collider> clonedCollider = std::dynamic_pointer_cast<Collider>(target);
    if (clonedCollider)
    {
        clonedCollider->mSnapShotIndex       = mSnapShotIndex.load();
        clonedCollider->mColliderSnapShot[0] = mColliderSnapShot[0];
        clonedCollider->mColliderSnapShot[1] = mColliderSnapShot[1];
        clonedCollider->mBS                  = mBS;
        clonedCollider->mBoundingBoxList.resize(mBoundingBoxList.size());
        std::memcpy(clonedCollider->mBoundingBoxList.data(), mBoundingBoxList.data(), mBoundingBoxList.size() * sizeof(MyBoundingOrientedBox));

    }

    return target;
}

void Collider::LateUpdate()
{
    UpdateTransform();

    /* Update Snap Shot */
    UpdateColliderSnapShot();
}

ColliderSnapShot Collider::GetSnapShot()
{
    ColliderSnapShot snapShot = mColliderSnapShot[mSnapShotIndex];
    return snapShot;

}


void Collider::UpdateColliderSnapShot()
{
    mColliderSnapShot[mSnapShotIndex].BS = mBS;
    mColliderSnapShot[mSnapShotIndex].BoundingBoxList.resize(mBoundingBoxList.size());

    std::memcpy(mColliderSnapShot[mSnapShotIndex].BoundingBoxList.data(), mBoundingBoxList.data(), sizeof(MyBoundingOrientedBox) * mBoundingBoxList.size());
    
    // [SnapShot Index] Swap!!!!
    mSnapShotIndex.store(!mSnapShotIndex);;

}

void Collider::SyncSnapShot()
{
    mColliderSnapShot[0].BS = mBS;
    mColliderSnapShot[0].BoundingBoxList.resize(mBoundingBoxList.size());
    std::memcpy(mColliderSnapShot[0].BoundingBoxList.data(), mBoundingBoxList.data(), sizeof(MyBoundingOrientedBox) * mBoundingBoxList.size());

    mColliderSnapShot[1].BS = mBS;
    mColliderSnapShot[1].BoundingBoxList.resize(mBoundingBoxList.size());
    std::memcpy(mColliderSnapShot[1].BoundingBoxList.data(), mBoundingBoxList.data(), sizeof(MyBoundingOrientedBox) * mBoundingBoxList.size());

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
