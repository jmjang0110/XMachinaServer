#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_LightBipedMech : public Script_Enemy
{
private:


public:
	Script_LightBipedMech();
	Script_LightBipedMech(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_LightBipedMech();



public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other);


};
