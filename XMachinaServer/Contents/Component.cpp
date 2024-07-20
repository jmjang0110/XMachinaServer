#include "pch.h"
#include "Component.h"


Component::Component()
{
}

Component::Component(const Component& other)
	: mType(other.mType)
{

}

Component::Component(SPtr<GameObject> owner, ComponentInfo::Type Type, UINT32 id)
	: GameEntity(id)

{
	mOwner = owner;
	mType = Type;

}

Component::~Component()
{
}

void Component::Activate()
{
	GameEntity::Activate();

	OnEnable();
}

void Component::DeActivate()
{
	GameEntity::DeActivate();

	OnDisable();
}

void Component::OnEnable()
{
	if (!mIsAwake) {
		WakeUp();
	}

	mIsActive = true;
}

void Component::OnDisable()
{
	mIsActive = false;
}

bool Component::WakeUp()
{
	mIsAwake = true;

	return true;
}

bool Component::Start()
{
	mIsStart = true;

	return true;
}

bool Component::Update()
{
	return true;
}

bool Component::LateUpdate()
{
	return false;
}

void Component::OnDestroy()
{
}

SPtr<Component> Component::Clone(SPtr<GameObject> copyOwner) const
{
	
	return SPtr<Component>();
}

