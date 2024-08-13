#pragma once
#include "Component.h"


namespace ScriptInfo {

	enum class Type : UINT16 {
		None, 

		Stat, // Enemy, Player 
		EnemyController,

		Skill,
		DefaultEnemyBT,
		MindControlledEnemyBT,
		DeusPhase1BT,

		Phero,
		PheroDropper,

		Building,

		Bullet,

		End,
	};

	
}
 

class Script : public Component
{
private:
	ScriptInfo::Type mType = ScriptInfo::Type::None;


public:
	Script();
	Script(SPtr<GameObject> owner, ScriptInfo::Type type, UINT32 id);
	virtual ~Script();

public:
	virtual void Clone(SPtr<Component> other) ;

	virtual void Activate();
	virtual void DeActivate();

	virtual void OnEnable();
	virtual void OnDisable();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual bool LateUpdate();
	virtual void OnDestroy();

};

