#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Rapax : public Script_Enemy
{
private:

public:
	Script_Rapax();
	Script_Rapax(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Rapax();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual bool Start() override;
	virtual void Clone(SPtr<Component> other);
};
