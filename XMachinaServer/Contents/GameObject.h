#pragma once
#include "GameEntity.h"
#include "Component.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Transform.h"
#include "Animation.h"
#include "Script.h"

class GameTimer;
class GameRoom;
class GameObject : public GameEntity
{
private:
	std::atomic<int>											mActivateRef = 0;

private:
	ObjectTag													mTag          = ObjectTag::Untagged;
	SPtr<GameRoom>												mOwnerRoom	  = {};
	GameTimer*													mTimer        = {};

	std::unordered_map<Component::Type,		SPtr<Component>>	mComponents   = {};	
	std::unordered_map<UINT32,				SPtr<Script>>		mScripts      = {};	
	SPtr<Script>												mScriptEntity = {};	

public:
	GameObject();
	GameObject(UINT32 sessionID);
	virtual ~GameObject();
public:
	virtual SPtr<GameObject> Clone() const;

	virtual void Start();	
	virtual void Update();
	virtual void LateUpdate();
	virtual void End();

	virtual void Activate();
	virtual void DeActivate();
	virtual void DecreaseActivateRef();

	virtual void	Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

public:
	bool			RegisterUpdate(float duration_second = 0.f); /// PQCS 를 이옹해 다음에 업데이트 할 것을 등록한다. 
	float			DeltaTime();
	
public:
	SPtr<Transform> GetTransform()				{ return GetComponent<Transform>(Component::Type::Transform); }
	SPtr<Collider>	GetCollider()				{ return GetComponent<Collider>(Component::Type::Collider); }
	SPtr<Animation> GetAnimation()				{ return GetComponent<Animation>(Component::Type::Animation); }
	SPtr<Rigidbody>	GetRigidbody()				{ return GetComponent<Rigidbody>(Component::Type::Rigidbody); }

	SPtr<GameRoom>	GetOwnerRoom()				{ return mOwnerRoom; }
	SPtr<Script>	GetScriptEntity()			{ return mScriptEntity; }
	template<typename T>
	SPtr<T> GetScriptEntity()					{ return mScriptEntity ? std::dynamic_pointer_cast<T>(mScriptEntity) : nullptr; }


	void SetOwnerRoom(SPtr<GameRoom> room)		{ mOwnerRoom    = room; }
	void SetScriptEntity(SPtr<Script> entity)	{ mScriptEntity = entity; }
	template<typename T>
	SPtr<T> SetScriptEntity()					{ mScriptEntity = MEMORY->Make_Shared<T>(std::dynamic_pointer_cast<GameObject>(shared_from_this())); return  std::dynamic_pointer_cast<T>(mScriptEntity); }

	/// --------------- TAG ---------------
	void		EnableTag(ObjectTag tag)		{ mTag.EnableTag(tag); }
	void		DisavleTag(ObjectTag tag)		{ mTag.DisableTag(tag); }
	bool		IsEnableTag(ObjectTag tag)		{ return mTag.IsTagEnabled(tag); }
	ObjectTag	GetTag()						{ return mTag.GetTag(); }

protected:
	void CloneComponents(SPtr<GameObject>& copy) const;
	void CloneScripts(SPtr<GameObject>& copy) const;

public:
	template<typename T>
	SPtr<T>  GetComponent(Component::Type componentTYpe);

	template<typename T>
	SPtr<T>  GetScript();

	template<typename T>
	SPtr<T>  AddComponent(Component::Type key);

	template<typename T>
	SPtr<T>  AddScript();

private:
	SPtr<Script>	AddScript();
	SPtr<Component> AddComponent(Component::Type key);

};

template<typename T>
inline SPtr<T>  GameObject::GetComponent(Component::Type componentType)
{
	auto it = mComponents.find(componentType);
	if (it != mComponents.end())
		return std::dynamic_pointer_cast<T>(it->second);
	else
		return nullptr;

}

template<typename T>
inline SPtr<T> GameObject::GetScript()
{
	// 임시 객체를 생성하여 key를 가져옴
	T tempScript;
	uint32_t key = tempScript.GetKey();

	// mScripts에서 해당 key로 스크립트를 찾음
	auto it = mScripts.find(key);
	if (it != mScripts.end()) {
		return std::dynamic_pointer_cast<T>(it->second);
	}
	else {
		return nullptr;
	}
}

template<typename T>
inline SPtr<T>  GameObject::AddComponent(Component::Type key)
{
	auto it = mComponents.find(key);
	if (it == mComponents.end()) {
		SPtr<T> component = MEMORY->Make_Shared<T>(std::dynamic_pointer_cast<GameObject>(shared_from_this()), key);
		mComponents.insert(std::make_pair(key, component));
		return component;
	}
	else {
		return std::dynamic_pointer_cast<T>(it->second);
	}
}

template<typename T>
inline SPtr<T>  GameObject::AddScript()
{
	SPtr<T>		script = MEMORY->Make_Shared<T>(std::dynamic_pointer_cast<GameObject>(shared_from_this()));
	uint32_t	key    = script->GetKey();

	auto it = mScripts.find(key);
	if (it == mScripts.end()) {
		mScripts.insert(std::make_pair(key, script));
		return script;
	}
	else {
		return std::dynamic_pointer_cast<T>(it->second);
	}
}

