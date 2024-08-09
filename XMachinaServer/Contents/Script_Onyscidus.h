#pragma once


#include "Script_Enemy.h"

class GameObject;
class Script_Onyscidus : public Script_Enemy
{
private:
	
;public:
	Script_Onyscidus();
	Script_Onyscidus(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Onyscidus();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual bool Start();
	virtual void Clone(SPtr<Component> other);
};

