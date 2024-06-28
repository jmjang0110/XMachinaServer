#pragma once
#include "GameEntity.h"

class Transform;

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
	Transform* mTransform = {};

protected:
	GameObjectInfo::Type	Type = GameObjectInfo::Type::None;


protected:
	bool IsActive = false;

public:
	GameObject();
	GameObject(UINT32 sessionID);
	virtual ~GameObject() override;

public:
	virtual void Update() {};
	virtual void WakeUp() {};

public:
	void SetType(GameObjectInfo::Type type) { Type = type; }

public:
	Transform* GetTransform() { return mTransform; }
};

