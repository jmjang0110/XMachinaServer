#pragma once
#include "Script.h"


class Script_Phero : public Script
{
private:
	int		mLevel    = {};
	float	mLifeTime = {};
	float	mAmount   = {};

	Vec3	mOffsetDist = {};

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
	void SetOffsetDist(Vec3 dist) { mOffsetDist = dist; }



};

