#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "GameTimer.h"
#include "Script.h"

GameObject::GameObject()
	: GameEntity()
{
	mTimer = new GameTimer;
}

GameObject::GameObject(UINT32 sessionID)
	: GameEntity(sessionID)
{
	mTimer = new GameTimer;

}

GameObject::~GameObject()
{
	SAFE_DELETE(mTimer);

	for (auto& iter : mComponents)
		iter.second = nullptr;
	for (auto& iter : mScripts)
		iter.second = nullptr;

	mScriptEntity = nullptr;
	mOwnerRoom    = nullptr;
}

void GameObject::Start()
{
	mTimer->Start();

	// Update all components
	for (auto& pair : mComponents) {
		if (pair.second) {
			pair.second->Start();
		}
	}

	// Update all scripts
	for (auto& pair : mScripts) {
		if (pair.second) {
			pair.second->Start();
		}
	}

	if (mComponents.end() != mComponents.find(Component::Type::Animation)) {
		GetAnimation()->Start();
	}

	if (mScriptEntity)
		mScriptEntity->Start();
}

void GameObject::Update()
{
	mTimer->Update();

	// Update all components
	for (auto& pair : mComponents) {
		if (pair.second) {  
			pair.second->Update();
		}
	}

	// Update all scripts
	for (auto& pair : mScripts) {
		if (pair.second) {  
			pair.second->Update();
		}
	}

	if (mComponents.end() != mComponents.find(Component::Type::Animation)) {
		GetAnimation()->Animate();
	}
		
	if (mScriptEntity)
		mScriptEntity->Update();

}

void GameObject::LateUpdate()
{
	for (auto& pair : mComponents) {
		if (pair.second) { 
			pair.second->LateUpdate();
		}
	}

	for (auto& pair : mScripts) {
		if (pair.second) {
			pair.second->LateUpdate();
		}
	}

	if (mScriptEntity)
		mScriptEntity->LateUpdate();

}

void GameObject::End()
{
}

void GameObject::Activate()
{
	mActivateRef.fetch_add(1);

	GameEntity::Activate();

	mTimer->Activate();

	for (auto& iter : mComponents) {
		iter.second->Activate();
	}

	for (auto& iter : mScripts) {
		iter.second->Activate();
	}

	if (mScriptEntity)
		mScriptEntity->Activate();
}

void GameObject::DeActivate()
{
	mActivateRef.fetch_sub(1);

	GameEntity::DeActivate();

	mTimer->DeActivate();

	for (auto& iter : mComponents) {
		iter.second->DeActivate();
	}
	for (auto& iter : mScripts) {
		iter.second->DeActivate();
	}

	if (mScriptEntity)
		mScriptEntity->DeActivate();
}

void GameObject::DecreaseActivateRef()
{
	mActivateRef.fetch_sub(1);

}

void GameObject::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	// 실질적인 처리를 Script 에 넘긴다. 
	if (mScriptEntity)
		mScriptEntity->Dispatch(overlapped, bytes);
}

bool GameObject::RegisterUpdate(float duration_second)
{
	
	TimerEvent t{};
	t.Type        = TimerEventType::Update_GameObject;
	t.WakeUp_Time = std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<float>(duration_second));
	t.Owner       = shared_from_this();
	
	TIME_MGR->PushTimerEvent(t);
	return true;
}

float GameObject::DeltaTime()
{
	return mTimer->DeltaTime();
}

SPtr<GameObject> GameObject::Clone() const
{
	SPtr<GameObject> copy = MEMORY->Make_Shared<GameObject>();
	copy->mID             = this->mID;
	copy->mName           = this->mName;
	CloneComponents(copy);
	CloneScripts(copy);

	return copy;
}

void GameObject::CloneComponents(SPtr<GameObject>& copy) const
{
	for (const auto& pair : mComponents)
	{
		SPtr<Component> component = copy->AddComponent(pair.first);
		component->SetOwner(copy);
		component->Clone(pair.second);
	}
}

void GameObject::CloneScripts(SPtr<GameObject>& copy) const
{
	/// 1. Clone Entity Script 
	mScriptEntity->Clone(copy);
	mScriptEntity->SetOwner(copy);
	
	/// 2. Clone Sub Scripts
	// Collect the script keys from the original GameObject and sort them
	std::vector<UINT32> sortedKeys;
	sortedKeys.reserve(mScripts.size());

	for (const auto& pair : mScripts) {
		sortedKeys.push_back(pair.first);
	}

	std::sort(sortedKeys.begin(), sortedKeys.end());

	// Clone each script and attach it to the new GameObject
	for (const auto& key : sortedKeys) {
		if (auto script = mScripts.at(key)) {
			script->Clone(copy);
			script->SetOwner(copy);

		}
	}


}

SPtr<Component> GameObject::AddComponent(Component::Type key)
{
	SPtr<Component> component{};

	switch (key)
	{
	case Component::Type::Transform: 
	{
		component = AddComponent<Transform>(key);
	}
		break;
	case Component::Type::Collider:
	{
		component = AddComponent<Collider>(key);
	}
		break;
	case Component::Type::Animation:
	{
		component = AddComponent<Animation>(key);
	}
		break;
	case Component::Type::Rigidbody:
	{
		component = AddComponent<Rigidbody>(key);
	}
		break;
	default:
		assert(0);
		break;
	}

	return component;
}