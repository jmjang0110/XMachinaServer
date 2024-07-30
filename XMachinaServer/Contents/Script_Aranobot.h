#pragma once
#include "Script_Enemy.h"

class GameObject;


class Script_Aranobot : public Script_Enemy
{
private:


public:
	Script_Aranobot();
	Script_Aranobot(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Aranobot();



public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other);


};
