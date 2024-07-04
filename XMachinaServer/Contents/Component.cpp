#include "pch.h"
#include "Component.h"


Component::Component()
{
}

Component::Component(UINT32 id)
	: GameEntity(id)
{
}

Component::~Component()
{
}

bool Component::WakeUp()
{
	return false;
}

bool Component::Start()
{
	return false;
}

bool Component::Update()
{
	return false;
}
