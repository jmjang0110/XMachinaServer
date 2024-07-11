#pragma once
#include "Script.h"



class Script_PheroDropper : public Script
{
private:


public:
	Script_PheroDropper();
	Script_PheroDropper(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_PheroDropper();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();



};

