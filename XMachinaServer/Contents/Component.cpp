#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Component::Component()
{
}

Component::Component(SPtr<GameObject> owner, Component::Type Type, UINT32 id)
{
	mOwner = owner;
	mType  = Type;
	mID    = id;
}

Component::~Component()
{
	mOwner = nullptr;
}

void Component::Activate()
{

}

void Component::DeActivate()
{
}

void Component::Start()
{
}

void Component::Update()
{
}

void Component::LateUpdate()
{
}

void Component::End()
{
}
float Component::DeltaTime()
{
	return mOwner->DeltaTime();
}

SPtr<Component> Component::Clone(SPtr<Component> target)
{
    // 타겟이 null인 경우 새 Component를 생성합니다.
    if (!target)
    {
        target = std::make_shared<Component>(mOwner, mType, mID);
    }

    // 복제할 속성을 타겟에 복사합니다.
    target->mID       = mID;
    target->mName     = mName;
    target->mType     = mType;
    target->mIsActive = mIsActive;

    return target;

}

