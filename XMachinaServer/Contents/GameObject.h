#pragma once
#include "GameEntity.h"
#include "Component.h"
#include "Script.h"
#include "Transform.h"

namespace GameObjectInfo
{
	enum class Type : UINT16 {
		None,

		GamePlayer,
		Monster_Ursacetus,
		Monster_Onyscidus,
		Monster_AdvancedCombat_5,

		END,
	};
}

class GameObject : public GameEntity
{
private:
	GameObjectInfo::Type										mType		= GameObjectInfo::Type::None;
	std::unordered_map<ComponentInfo::Type, SPtr<Component>>	mComponents = {};
	std::unordered_map<ScriptInfo::Type,    SPtr<Script>>		mScripts    = {};


public:
	GameObject();
	GameObject(UINT32 sessionID);
	virtual ~GameObject();

public:
	virtual void Update() {};
	virtual void WakeUp() {};

	virtual void Activate();
	virtual void DeActivate();

public:
	void SetType(GameObjectInfo::Type type) { mType = type; }


public:
	/* Util */
	SPtr<Transform> GetTransform() { return GetComponent<Transform>(ComponentInfo::Type::Transform); }

public:
	template<typename T>
	SPtr<T>  GetComponent(ComponentInfo::Type componentTYpe);

	template<typename T>
	SPtr<T>  GetScript(ScriptInfo::Type scriptType);

public:
	template<typename T>
	SPtr<T>  AddComponent(ComponentInfo::Type key);

	template<typename T>
	SPtr<T>  AddScript(ScriptInfo::Type key);

};

template<typename T>
inline SPtr<T>  GameObject::GetComponent(ComponentInfo::Type componentType)
{
	auto it = mComponents.find(componentType);
	if (it != mComponents.end())
		return std::dynamic_pointer_cast<T>(it->second);
	else
		return nullptr;

}

template<typename T>
inline SPtr<T> GameObject::GetScript(ScriptInfo::Type scriptType)
{
	auto it = mScripts.find(scriptType);
	if (it != mScripts.end())
		return std::dynamic_pointer_cast<T>(it->second);
	else
		return nullptr;
}

template<typename T>
inline SPtr<T>  GameObject::AddComponent(ComponentInfo::Type key)
{
	if (mComponents.find(key) != mComponents.end())
		mComponents.insert(std::make_pair<ComponentInfo::Type, SPtr<T>>(key, MEMORY->Make_Shared<T>()));
	else
		return mComponents.find(key)->second;
}

template<typename T>
inline SPtr<T>  GameObject::AddScript(ScriptInfo::Type key)
{
	auto it = mScripts.find(key);
	if (it == mScripts.end()) {
		SPtr<T> script = MEMORY->Make_Shared<T>();
		mScripts.insert(std::make_pair(key, script));
		return script;
	}
	else {
		return std::dynamic_pointer_cast<T>(it->second);
	}
}
