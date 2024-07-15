#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "ServerLib/ServerNetwork.h"
#include "Framework.h"

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

bool GameObject::RegisterUpdate(std::chrono::system_clock::duration offset)
{

	TimerEvent t;
	t.Type = TimerEventType::Update_GameObject;
	t.WakeUp_Time = std::chrono::system_clock::now() + offset; 
	t.Owner = shared_from_this();
	TIME_MGR->PushTimerEvent(t);

	return true;

}

