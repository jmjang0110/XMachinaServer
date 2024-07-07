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
	GameObjectInfo::Type	mType							= GameObjectInfo::Type::None;
	Component*				mComponents[ComponentType::End];
	Script*					mScripts[ScriptType::End];

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
	T* GetComponent(int32_t componentTYpe);
	template<typename T>
	T* GetScript(int32_t scriptType);


};

template<typename T>
inline T* GameObject::GetComponent(int32_t componentTYpe)
{
	return reinterpret_cast<T*>(mComponents[componentTYpe]);
}

template<typename T>
inline T* GameObject::GetScript(int32_t scriptType)
{
	return reinterpret_cast<T*>(mScripts[scriptType]);
}
