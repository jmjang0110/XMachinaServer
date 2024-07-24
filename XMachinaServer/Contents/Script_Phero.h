#pragma once
#include "Script.h"


class Script_Phero : public Script
{
private:
	std::string mPheroState = {}; // ex) "123" : Phero Level : 1, OffsetDistIndex = 23 ( PheroDropInfo::Offsets[23] )
private:
	int		mLevel      = {};
	float	mLifeTime   = {};
	float	mAmount     = {};

	int mOffsetDistIndex = {};

public:
	Script_Phero();
	Script_Phero(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Phero();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other) ;

	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();

public:
	void Init(int level, float lifeTime, float amount);
	void Init();

	void SetLevel(int level) { mLevel = level; }

public:
	void SetOffsetDistIndex(int index) { mOffsetDistIndex = index; }
	std::string& GetPheroStateString() { return mPheroState; }
	void setPheroStateString(std::string state) { mPheroState = state; }


};

