#pragma once
#include "Script.h"

class Script_Building : public Script
{
private:
	Coordinate mSectorIdx;


public:
	void		SetSectorIdx(Coordinate& sectorIdx) { mSectorIdx = sectorIdx; }
	Coordinate	GetSectorIdx()						{ return mSectorIdx; }


public:
	Script_Building();
	Script_Building(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Building();

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
};

