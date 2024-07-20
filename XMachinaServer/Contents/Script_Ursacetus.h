#pragma once
#include "Script_Enemy.h"

class GameObject;

class Script_Ursacetus : public Script_Enemy
{
private:


public:
	Script_Ursacetus();
	Script_Ursacetus(SPtr<GameObject> owner, ScriptInfo::Type type);
	virtual ~Script_Ursacetus();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual SPtr<Component> Clone(SPtr<GameObject> copyOwner) const;

	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();

	/// +------------------------------
	///		Stat :  virtual function 
	/// ------------------------------+
	virtual void Attack();
	virtual void AttackCallback();
	virtual void Dead();
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;


};

