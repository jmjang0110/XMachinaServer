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
    // mBoundingSphereList ����
    mBoundingSphereList.resize(other.mBoundingSphereList.size());
    if (!other.mBoundingSphereList.empty()) {
        memcpy(mBoundingSphereList.data(), other.mBoundingSphereList.data(),
            other.mBoundingSphereList.size() * sizeof(MyBoundingSphere));
    }

    // mBoundingBoxList ����
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

    // Create a new Collider object
    SPtr<Collider> collider = std::static_pointer_cast<Collider>(other);

    // Copy the bounding sphere list
    this->mBoundingSphereList.resize(collider->mBoundingSphereList.size());
    std::memcpy(collider->mBoundingSphereList.data(), collider->mBoundingSphereList.data(), collider->mBoundingSphereList.size() * sizeof(MyBoundingSphere));

    // Copy the bounding box list
    this->mBoundingBoxList.resize(collider->mBoundingBoxList.size());
    std::memcpy(collider->mBoundingBoxList.data(), collider->mBoundingBoxList.data(), collider->mBoundingBoxList.size() * sizeof(MyBoundingOrientedBox));

    // Associate the new Collider with the copyOwner
    this->SetType(ComponentInfo::Type::Collider);

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
//    // �ݵ�� �� ��ü ��� ObjectCollider�� ������ �־�� �Ѵ�.
//    if (!colliderA || !colliderB) {
//        return false;
//    }
//
//    // �� �� �ϳ��� ��Ȱ�� ���¶�� �˻����� �ʴ´�.
//    if (!colliderA->IsActive() || !colliderB->IsActive()) {
//        return false;
//    }
//
//    return colliderA->Intersects(colliderB);
//}

