#pragma once

#include "Script.h"
#include "GameObject.h"
#include "GamePlayer.h"
#include "GameMonster.h"


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
	SPtr<GameMonster>	mOwnerMonster;
private:
	bool				mMindControllded = false;
	EnemyInfo::State	mState  = EnemyInfo::State::Idle;

	SPtr_GameObject		mTarget     = {}; 
	SPtr_GameObject		mPathTarget = {};
	std::stack<Vec3>	mPaths      = {};

	/* Mind Control Off */
	SPtr<GamePlayer>	mTargetPlayer = {};

	/* Min Control On */
	SPtr<GameMonster>   mTargetMonster = {};

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other) ;

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
	//SPtr_GameObject		GetTargetObject()					{ return mTarget;	   }
	SPtr<GamePlayer>    GetTargetPlayer()					{ return mTargetPlayer; }
	SPtr<GameMonster>	GetTargetMonster()					{ return mTargetMonster;  }
	SPtr_GameObject		GetPathTargetObject()				{ return mPathTarget;  }

	EnemyInfo::State	GetState()							{ return mState; }
	void				SetState(EnemyInfo::State state)	{ mState = state; }
	std::stack<Vec3>*	GetPaths()							{ return &mPaths; }

	void OnMindControl() { mMindControllded  = true; }
	void OffMindControl() { mMindControllded = false; }

	bool IsMindControlled() { return mMindControllded; }

public:
	//void SetTargetObject(SPtr_GameObject target)		{ mTarget = target; }
	void SetPathTargetObject(SPtr<GameObject> target)	{ mPathTarget = target; }

	void SetTargetPlayer(SPtr<GamePlayer> target)		{ mTargetPlayer = target; }
	void SetTargetMonster(SPtr<GameMonster> target)		{ mTargetMonster = target; }
	
	void SetOwnerMonster(SPtr<GameMonster> ownerMonster) { mOwnerMonster = ownerMonster; }
	SPtr<GameMonster> GetOwnerMonster() { return mOwnerMonster; }

};

