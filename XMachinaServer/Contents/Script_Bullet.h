#pragma once
#include "Script.h"

class Script_Bullet : public Script
{
private:

public:
	Script_Bullet();
	Script_Bullet(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Bullet();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other);

	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();
};

