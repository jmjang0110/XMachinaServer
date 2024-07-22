#pragma once
#include "GameEntity.h"
#include "Component.h"
#include "Script.h"
#include "Transform.h"
#include "Collider.h"
#include "Animation.h"


class GameObject : public GameEntity
{
private:
	GameObjectInfo::Type										mType		= GameObjectInfo::Type::None;
	std::unordered_map<ComponentInfo::Type, SPtr<Component>>	mComponents = {};
	std::unordered_map<ScriptInfo::Type,    SPtr<Script>>		mScripts    = {};
	SPtr<Animation> mAnimation{};

public:
	GameObject();
	GameObject(UINT32 sessionID);
	virtual ~GameObject();

public:
	virtual void WakeUp();
	virtual void Update();
	virtual void Start();



	virtual void Activate();
	virtual void DeActivate();

	// ��ü(other)�� �浹 �� ȣ��ȴ�.
	virtual void OnCollision(GameObject* other);

public:
	void SetType(GameObjectInfo::Type type) { mType = type; }
	GameObjectInfo::Type GetType() { return mType; }
	bool RegisterUpdate(std::chrono::system_clock::duration offset = std::chrono::seconds(0)); /// PQCS �� �̿��� ������ ������Ʈ �� ���� ����Ѵ�. 

	void SetAnimation(const std::string& controller);

public:
	/* Util */
	SPtr<Transform> GetTransform() { return GetComponent<Transform>(ComponentInfo::Type::Transform); }
	SPtr<Collider> GetCollider() { return GetComponent<Collider>(ComponentInfo::Type::Collider); }
	SPtr<Animation> GetAnimation() { return GetComponent<Animation>(ComponentInfo::Type::Animation); }

public:
	// ���� ���� �Լ� ����
	virtual SPtr<GameObject> Clone() const;

protected:
	void CloneComponents(SPtr<GameObject>& copy) const;
	void CloneScripts(SPtr<GameObject>& copy) const;

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

private:
	SPtr<Script> AddScript(ScriptInfo::Type key);
	SPtr<Component> AddComponent(ComponentInfo::Type key);

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
inline SPtr<T>  GameObject::AddScript(ScriptInfo::Type key)
{
	auto it = mScripts.find(key);
	if (it == mScripts.end()) {
		SPtr<T> script = MEMORY->Make_Shared<T>(std::dynamic_pointer_cast<GameObject>(shared_from_this()), key);
		mScripts.insert(std::make_pair(key, script));
		return script;
	}
	else {
		return std::dynamic_pointer_cast<T>(it->second);
	}
}
