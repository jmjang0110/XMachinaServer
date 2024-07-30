#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Gobbler : public Script_Enemy
{
private:


public:
	Script_Gobbler();
	Script_Gobbler(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Gobbler();



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

	/// +------------------------------
///		Stat :  virtual function 
/// ------------------------------+
	virtual void AttackCallback();
	virtual void Dead();
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;

public:



};
