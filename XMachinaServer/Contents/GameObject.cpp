#include "pch.h"
#include "GameObject.h"
#include "Transform.h"


GameObject::GameObject()
{
	mTransform = new Transform;
}

GameObject::GameObject(UINT32 sessionID)
	: GameEntity(sessionID)
{
	mTransform = new Transform;

}

GameObject::~GameObject()
{
	SAFE_DELETE(mTransform);
}