#pragma once
#include "GameEntity.h"
#include "Component.h"
#include "Script.h"

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
	GameObjectInfo::Type	mType		= GameObjectInfo::Type::None;

	std::unordered_map<ComponentInfo::Type, Component*> mComponents = {};
	std::unordered_map<ScriptInfo::Type, Script*> mScripts = {};


public:
	GameObject();
	GameObject(UINT32 sessionID);
	virtual ~GameObject() ;

public:
	virtual void Update() {};
	virtual void WakeUp() {};

public:
	void SetType(GameObjectInfo::Type type) { mType = type; }

public:
	template<typename T>
	T* GetComponent(ComponentInfo::Type componentTYpe);
	template<typename T>
	T* GetScript(ScriptInfo::Type scriptType);

public:
	template<typename T>
	T* AddComponent(ComponentInfo::Type key);

	template<typename T>
	T* AddScript(ScriptInfo::Type key);

};

template<typename T>
inline T* GameObject::GetComponent(ComponentInfo::Type componentTYpe)
{
	if (mComponents.find(componentTYpe) != mComponents.end())
		return reinterpret_cast<T*>(mComponents[componentTYpe]);
	else
		return nullptr;

}

template<typename T>
inline T* GameObject::GetScript(ScriptInfo::Type scriptType)
{
	if (mScripts.find(scriptType) != mScripts.end())
		return reinterpret_cast<T*>(mScripts[scriptType]);
	else
		return nullptr;
}

template<typename T>
inline T* GameObject::AddComponent(ComponentInfo::Type key)
{
	if (mComponents.find(key) != mComponents.end())
		mComponents.insert(std::make_pair<ComponentInfo::Type, T*>(key, MEMORY->New<T>()));
	else
		return mComponents.find(key)->second;
}

template<typename T>
inline T* GameObject::AddScript(ScriptInfo::Type key)
{
	if (mScripts.find(key) != mScripts.end())
		mScripts.insert(std::make_pair<ScriptInfo::Type, T*>(key, MEMORY->New<T>()));
	else
		return mScripts.find(key)->second;
}
