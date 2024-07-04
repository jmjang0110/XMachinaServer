#include "pch.h"
#include "Collider.h"

Collider::Collider()
{
}

Collider::Collider(UINT32 id)
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
