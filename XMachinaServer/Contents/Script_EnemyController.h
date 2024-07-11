#pragma once

#include "Script.h"
#include "GameObject.h"


/// +-------------------------------
///		     Script_EnemyController 
/// ________________________________
///	> [ 설명 ] 
///		- Enemy Controller Script를 갖고 있는 객체는 
///		- 자신이 Enemy 이며 
/// ________________________________
/// 
/// 

namespace EnemyInfo
{
	struct Stat 
	{

	};

	enum class State : UINT8 {
		Idle = 0,
		Walk,
		GetHit,
		Attack,
		Death,
		END,
	};
}


class Script_EnemyController : public Script
{
private:
	EnemyInfo::State	mState  = EnemyInfo::State::Idle;

	SPtr_GameObject		mTarget     = {}; 
	SPtr_GameObject		mPathTarget = {};
	std::stack<Vec3>	mPaths      = {};


public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;
	virtual void OnDestroy() override;

public:
	void Reset();
	void ForceSetTarget(SPtr_GameObject target) { mTarget = target; }

public:
	Script_EnemyController();
	Script_EnemyController(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_EnemyController();

public:
	SPtr_GameObject GetTargetObject()		{ return mTarget;	  }
	SPtr_GameObject GetPathTargetObject()	{ return mPathTarget; }

	EnemyInfo::State GetState() { return mState; }
	void SetState(EnemyInfo::State state) { mState = state; }
	std::stack<Vec3>*	GetPaths() { return &mPaths; }

public:
	void SetTargetObject(SPtr_GameObject target) { mTarget = target; }


};

