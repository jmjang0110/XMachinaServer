#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_MiningMech : public Script_Enemy
{
private:


public:
	Script_MiningMech();
	Script_MiningMech(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_MiningMech();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual bool Start() override;
	virtual void Clone(SPtr<Component> other);
};
