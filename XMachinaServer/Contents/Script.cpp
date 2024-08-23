#include "pch.h"
#include "Script.h"

#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "Animation.h"
#include "Rigidbody.h"

Script::Script()
	: Component()
{
}

Script::Script(SPtr<GameObject> owner, UINT32 id)
	: Component(owner, Component::Type::Script, id)
{
    mKey = id;
}

Script::~Script()
{
}

void Script::Clone(SPtr<GameObject> target)
{
    mOwner = target;
}

SPtr<Component> Script::Clone(SPtr<Component> target)
{
    // First, clone the base Component part
    Component::Clone(target);

    // Cast the target to the appropriate type (Script)
    auto script = std::dynamic_pointer_cast<Script>(target);

    // Ensure the casting was successful
    if (script)
    {
        // Copy the script-specific member variables
        script->mKey = this->mKey;
        // Add any additional copying logic if needed
    }

    return target;
}

void Script::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}

SPtr<Transform> Script::OwnerTransform()
{
	return mOwner->GetComponent<Transform>(Component::Type::Transform);
}
SPtr<Collider>	Script::OwnerCollider()
{
	return mOwner->GetComponent<Collider>(Component::Type::Collider);

}
SPtr<Animation> Script::OwnerAnimation()
{
	return mOwner->GetComponent<Animation>(Component::Type::Animation);

}
SPtr<Rigidbody>	Script::OwnerRigidbody()
{
	return mOwner->GetComponent<Rigidbody>(Component::Type::Rigidbody);

}