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
	Transform* GetTransform() { return mTransform; }
};

