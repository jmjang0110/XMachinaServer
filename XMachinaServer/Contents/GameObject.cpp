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

void GameObject::Activate()
{
	for (auto& iter : mComponents) {
		iter.second->Activate();
	}

	for (auto& iter : mScripts) {
		iter.second->Activate();
	}
}

void GameObject::DeActivate()
{
	for (auto& iter : mComponents) {
		iter.second->DeActivate();
	}
	for (auto& iter : mScripts) {
		iter.second->DeActivate();
	}

}
