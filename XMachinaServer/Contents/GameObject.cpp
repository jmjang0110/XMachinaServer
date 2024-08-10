#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "ServerLib/ServerNetwork.h"
#include "Framework.h"
#include "Component.h"
#include "Script.h"
#include "Transform.h"
#include "Collider.h"
#include "Animation.h"

/* Script Headers */
#include  "Script_BehaviorTree.h"
#include "Script_Building.h"
#include "Script_DefaultEnemyBT.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
#include "Script_EnemyStat.h"
#include "Script_Phero.h"
#include "Script_PheroDropper.h"
#include "Script_Player.h"
#include "Script_PlayerStat.h"
#include "Script_Stat.h"
#include "Script_Bullet.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Ursacetus.h"
#include "Script_Onyscidus.h"
#include "Script_Arack.h"
#include "Script_Aranobot.h"
#include "Script_Ceratoferox.h"
#include "Script_Gobbler.h"
#include "Script_LightBipedMech.h"
#include "Script_Rapax.h"
#include "Script_Anglerox.h"
#include "Script_MiningMech.h"


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

	UpdateDeltaTime();
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

	if (mComponents.end() != mComponents.find(ComponentInfo::Type::Animation))
		mComponents.at(ComponentInfo::Type::Animation)->Animate();
	

	for (auto& pair : mComponents) {
		if (pair.second) {  // Check if the shared pointer is not null
			pair.second->LateUpdate();
		}
	}

	for (auto& pair : mScripts) {
		if (pair.second) {
			pair.second->LateUpdate();
		}
	}
}

void GameObject::Animate()
{
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
	GameEntity::Activate();

	mCurrTimePoint = std::chrono::steady_clock::now();

	for (auto& iter : mComponents) {
		iter.second->Activate();
	}

	for (auto& iter : mScripts) {
		iter.second->Activate();
	}
}

void GameObject::DeActivate()
{
	GameEntity::DeActivate();

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

void GameObject::UpdateDeltaTime()
{
	mPrevTimePoint = mCurrTimePoint;
	mCurrTimePoint = std::chrono::steady_clock::now();

	std::chrono::duration<double> elapsed_seconds = mCurrTimePoint - mPrevTimePoint;
	mDeltaTime = static_cast<float>(elapsed_seconds.count());

}

bool GameObject::RegisterUpdate(std::chrono::system_clock::duration offset)
{

	TimerEvent t;
	t.Type        = TimerEventType::Update_GameObject;
	t.WakeUp_Time = std::chrono::system_clock::now() + offset; 
	t.Owner       = shared_from_this();
	TIME_MGR->PushTimerEvent(t);

	return true;

}

void GameObject::SetAnimation(const std::string& controller)
{
	if (controller == "") {
		return;
	}

	SPtr<Animation> animation = AddComponent<Animation>(ComponentInfo::Type::Animation);
	animation->Load(controller);
}


SPtr<GameObject> GameObject::Clone() const
{

	SPtr<GameObject> copy = MEMORY->Make_Shared<GameObject>();

	copy->ID    = this->ID;
	copy->mType = this->mType;

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
	auto keys = mScripts | std::views::keys;
	std::vector<ScriptInfo::Type> sortedKeys(keys.begin(), keys.end());
	std::ranges::sort(sortedKeys);
	for (auto key : sortedKeys)
	{
		SPtr<Script> script = copy->AddScript(key);
		script->SetOwner(copy);
		script->Clone(mScripts.at(key));
	}

}



SPtr<Script> GameObject::AddScript(ScriptInfo::Type key)
{
	SPtr<Script> script{};

	switch (key)
	{
	case ScriptInfo::Type::Stat: {
		switch (mType)
		{
		case GameObjectInfo::Type::Monster_Ursacetus:
			script = AddScript<Script_Ursacetus>(key);

			break;
		case GameObjectInfo::Type::Monster_Onyscidus:
			script = AddScript<Script_Onyscidus>(key);

			break;
		case GameObjectInfo::Type::Monster_AdvancedCombat_5:
			script = AddScript<Script_AdvancedCombatDroid_5>(key);
			break;
		case GameObjectInfo::Type::Monster_Anglerox:
			script = AddScript<Script_Anglerox>(key);

			break;
		case GameObjectInfo::Type::Monster_Arack:
			script = AddScript<Script_Arack>(key);

			break;
		case GameObjectInfo::Type::Monster_Ceratoferox:
			script = AddScript<Script_Ceratoferox>(key);

			break;
		case GameObjectInfo::Type::Monster_Gobbler:
			script = AddScript<Script_Gobbler>(key);

			break;
		case GameObjectInfo::Type::Monster_LightBipedMech:
			script = AddScript<Script_LightBipedMech>(key);

			break;
		case GameObjectInfo::Type::Monster_MiningMech:
			script = AddScript<Script_MiningMech>(key);

			break;
		case GameObjectInfo::Type::Monster_Rapax:
			script = AddScript<Script_Rapax>(key);

			break;
		case GameObjectInfo::Type::Monster_Aranobot:
			script = AddScript<Script_Aranobot>(key);

			break;
		default:
			break;
		}
	}
		break;
	case ScriptInfo::Type::EnemyController:
		script = AddScript<Script_EnemyController>(key);
		break;
	
	case ScriptInfo::Type::DefaultEnemyBT:
		script = AddScript<Script_DefaultEnemyBT>(key);
		break;
	case ScriptInfo::Type::Phero:
		script = AddScript<Script_Phero>(key);
		break;
	case ScriptInfo::Type::PheroDropper:
		script = AddScript<Script_PheroDropper>(key);
		break;
	case ScriptInfo::Type::Building:
		script = AddScript<Script_Building>(key);
		break;
	case ScriptInfo::Type::Bullet:
		script = AddScript<Script_Bullet>(key);
		break;
	default:
		break;
	}

	return script;

}


SPtr<Component> GameObject::AddComponent(ComponentInfo::Type key)
{
	SPtr<Component> component{};

	switch (key)
	{
	case ComponentInfo::Type::Transform: 
	{
		component = AddComponent<Transform>(key);
	}
		break;
	case ComponentInfo::Type::Collider:
	{
		component = AddComponent<Collider>(key);
	}
		break;
	case ComponentInfo::Type::Animation:
	{
		component = AddComponent<Animation>(key);
	}
	break;
	default:
		assert(0);
		break;
	}

	return component;
}