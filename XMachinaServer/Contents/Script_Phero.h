#pragma once
#include "Script.h"


class Script_Phero : public Script
{
private:
	int					mLevel      = {};
	float				mLifeTime   = {};
	float				mAmount     = {};

	std::atomic<int>	mTargetPlayer = -1;

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
public:
	int GetTargetPlayerID()			{ int targetID = mTargetPlayer.load(); return targetID; }
	float GetAmount()				{ return mAmount; }

	void SetLevel(int level)		{ mLevel = level; }
	void SetTargetPlayerID(int id)	{ if (mTargetPlayer.load() == -1) mTargetPlayer.store(id); }


	int GetLevel() { return mLevel; }

};

