#pragma once
#include "GameEntity.h"

class Transform;

class GameObject : public GameEntity
{
private:
	Transform* mTransform = {};

public:
	GameObject();
	GameObject(UINT32 sessionID);
	virtual ~GameObject() override;

public:
	virtual void Update() {};
	virtual void WakeUp() {};


public:
	Transform* GetTransform() { return mTransform; }
};

