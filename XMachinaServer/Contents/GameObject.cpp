#include "pch.h"
#include "GameObject.h"
#include "Transform.h"


GameObject::GameObject()
	: GameEntity()
{
}

GameObject::GameObject(UINT32 sessionID)
	: GameEntity(sessionID)
{

}

GameObject::~GameObject()
{
}