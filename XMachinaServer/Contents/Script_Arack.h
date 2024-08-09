#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Arack : public Script_Enemy
{
private:
public:
	Script_Arack();
	Script_Arack(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Arack();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual bool Start();
	virtual void Clone(SPtr<Component> other);
};
