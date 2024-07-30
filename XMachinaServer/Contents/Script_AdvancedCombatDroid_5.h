#pragma once
#include "Script_Enemy.h"

class GameObject;


class Script_AdvancedCombatDroid_5 : public Script_Enemy
{
private:


public:
	Script_AdvancedCombatDroid_5();
	Script_AdvancedCombatDroid_5(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_AdvancedCombatDroid_5();


	
public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other) ;

};

