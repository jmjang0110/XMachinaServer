#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "ServerLib/ServerNetwork.h"
#include "Framework.h"
#include "Component.h"
#include "Script.h"
#include "Transform.h"
#include "Collider.h"


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

void GameObject::Update()
{
	// Update all components
	for (auto& pair : mComponents) {
		if (pair.second) {  // Check if the shared pointer is not null
			pair.second->Update();
		}
	}

	// Update all scripts
	for (auto& pair : mScripts) {
		if (pair.second) {  // Check if the shared pointer is not null
			pair.second->Update();
		}
	}

	for (auto& pair : mComponents) {
		if (pair.second) {  // Check if the shared pointer is not null
			pair.second->LateUpdate();
		}
	}

}

void GameObject::WakeUp()
{
	for (auto& iter : mComponents) {
		iter.second->WakeUp();
	}

	for (auto& iter : mScripts) {
		iter.second->WakeUp();
	}
}

void GameObject::Start()
{
	for (auto& iter : mComponents) {
		iter.second->Start();
	}

	for (auto& iter : mScripts) {
		iter.second->Start();
	}
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

void GameObject::OnCollision(GameObject* other)
{
}

bool GameObject::RegisterUpdate(std::chrono::system_clock::duration offset)
{

	TimerEvent t;
	t.Type = TimerEventType::Update_GameObject;
	t.WakeUp_Time = std::chrono::system_clock::now() + offset; 
	t.Owner = shared_from_this();
	TIME_MGR->PushTimerEvent(t);

	return true;

}

SPtr<GameObject> GameObject::DeepCopy() const
{
	SPtr<GameObject> copy = MEMORY->Make_Shared<GameObject>();

	copy->mType = this->mType;

	for (const auto& pair : mComponents)
	{
		copy->mComponents[pair.first] = pair.second->Clone(copy); // Assume Component has a Clone() method
		copy->mComponents[pair.first]->SetOwner(copy);

	}

	for (const auto& pair : mScripts)
	{
		copy->mScripts[pair.first] = dynamic_pointer_cast<Script>(pair.second->Clone(copy)); // Assume Script has a Clone() method
		copy->mScripts[pair.first]->SetOwner(copy);

	}

	return copy;
}

