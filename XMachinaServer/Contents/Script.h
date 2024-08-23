#pragma once
#include "Component.h"

class Transform;
class Collider;
class Animation;
class Rigidbody;
class GameObject;
class Script : public Component
{
protected:
	UINT32	mKey   = -1;

public:
	Script();
	Script(SPtr<GameObject> owner, UINT32 key);
	virtual ~Script();

public:
	virtual void Clone(SPtr<GameObject> target);
	virtual SPtr<Component> Clone(SPtr<Component> target);
	template<typename T>
	SPtr<T> GetDerivedScript() { return std::dynamic_pointer_cast<T>(shared_from_this()); }

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0);


public:
	SPtr<Transform> OwnerTransform();
	SPtr<Collider>	OwnerCollider()	;
	SPtr<Animation> OwnerAnimation();
	SPtr<Rigidbody>	OwnerRigidbody();

	UINT32 GetKey() { return mKey; }
};

