#include "pch.h"
#include "Collider.h"

Collider::Collider()
{
}

Collider::Collider(SPtr<GameObject> owner, ComponentInfo::Type Type)
    : Component(owner, ComponentInfo::Type::Collider, static_cast<UINT32>(ComponentInfo::Type::Collider))
{
}

Collider::~Collider()
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
