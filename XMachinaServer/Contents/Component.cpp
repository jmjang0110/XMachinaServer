#include "pch.h"
#include "Component.h"


Component::Component()
{
}

Component::Component(UINT32 id, ComponentInfo::Type Type)
	: GameEntity(id)
{
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

void Component::OnDestroy()
{
}
