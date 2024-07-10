#pragma once

#include "GameEntity.h"

class GameObject;

namespace ComponentInfo {

	enum class Type : UINT16 {
		None ,

		Transform,
		Collider,

		Script,
		End,

	};
}



class Component : public GameEntity
{
private:
	ComponentInfo::Type mType = ComponentInfo::Type::None;
	SPtr_GameObject mOwner = nullptr;

private:
	bool mIsAwake  = false;
	bool mIsStart  = false;
	bool mIsActive = false;

public:
	Component();
	Component(UINT32 id, ComponentInfo::Type Type);
	virtual ~Component();

public:
	virtual void Activate();
	virtual void DeActivate();

	virtual void OnEnable();
	virtual void OnDisable();

public:
	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();

public:
	void		SetOwner(SPtr_GameObject owner) { mOwner = owner; }
	SPtr_GameObject GetOwner() { return mOwner; }

public:
	bool IsAwake() const { return mIsAwake; }
	bool IsStart() const { return mIsStart; }
	bool IsActive() const { return mIsActive; }
};

