#pragma once

#include "Script_Enemy.h"

class GameObject;

class Script_Anglerox : public Script_Enemy
{
private:


public:
	Script_Anglerox();
	Script_Anglerox(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Anglerox();



public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual bool Start() override;
	virtual void Clone(SPtr<Component> other) override;

};


