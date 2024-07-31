#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Ceratoferox : public Script_Enemy
{
private:

public:
	Script_Ceratoferox();
	Script_Ceratoferox(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Ceratoferox();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other);


};
