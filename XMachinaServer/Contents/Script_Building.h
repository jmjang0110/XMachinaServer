#pragma once
#include "Script_Entity.h"

class Script_Building : public Script_Entity
{
private:
	Coordinate mSectorIdx;


public:
	void		SetSectorIdx(Coordinate& sectorIdx) { mSectorIdx = sectorIdx; }
	Coordinate	GetSectorIdx()						{ return mSectorIdx; }


public:
	Script_Building();
	Script_Building(SPtr<GameObject> owner);
	~Script_Building();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);


};

