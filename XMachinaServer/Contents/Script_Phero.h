#pragma once
#include "Script_Entity.h"

namespace PheroInfo {
	constexpr uint32_t MaxPheroCreateCount = 15;
}

class Script_Phero : public Script_Entity
{
private:
	int					mLevel      = {};
	float				mLifeTime   = {};
	float				mAmount     = {};
	Vec3				mOffsetDist = {};

	std::atomic<int>	mTargetPlayerID = -1;

public:
	Script_Phero();
	Script_Phero(SPtr<GameObject> owner);
	virtual ~Script_Phero();

public:

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Activate();
	virtual void DeActivate();

	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void End();


public:
	void Init(int monster_id, int phero_index, int level, float lifeTime, float amount);

public:
	/// ----------------------- Get -----------------------
	int		GetTargetPlayerID()			{ int targetID = mTargetPlayerID.load(); return targetID; }
	float	GetAmount()					{ return mAmount; }
	Vec3	GetOffsetDist()				{ return mOffsetDist; }
	int		GetLevel()					{ return mLevel; }

	/// ----------------------- Set -----------------------
	void	SetLevel(int level)			{ mLevel      = level; }
	void	SetOffsetDist(Vec3 dist)	{ mOffsetDist = dist; }
	void	SetTargetPlayerID(int id)	{ if (mTargetPlayerID.load() == -1) mTargetPlayerID.store(id); }

};

