#pragma once

#include "Script.h"
#include "GameObject.h"
#include "GamePlayer.h"
#include "GameMonster.h"
#include "Animation.h"

/// +-------------------------------
///		     Script_EnemyController 
/// ________________________________
///	> [ ���� ] 
///		- Enemy Controller Script�� ���� �ִ� ��ü�� 
///		- �ڽ��� Enemy �̸� 
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
	SPtr<GamePlayer>	mTargetPlayer = {}; Lock::SRWLock mLock_TargetPlayer;

	/* Min Control On */
	SPtr<GameMonster>   mTargetMonster = {};


	FBProtocol::MONSTER_BT_TYPE mCurrBTType;
	FBProtocol::MONSTER_BT_TYPE mPrevBTType;
	FBProtocol::MONSTER_BT_TYPE mBTType; Lock::SRWLock mLock_BTType;


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
	Script_EnemyController();
	Script_EnemyController(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_EnemyController();

public:
	void Reset();
	void RemoveAllAnimation();
	void ForceSetTarget(SPtr_GameObject target) { mTarget = target; }

public:
	/// +---------------------------------------------------
	///						G E T T E R  
	/// ---------------------------------------------------+
	SPtr<GameMonster>			GetOwnerMonster()			{ return mOwnerMonster; }

	SPtr<GamePlayer>			GetTargetPlayer() { mLock_TargetPlayer.LockRead(); SPtr<GamePlayer> player = mTargetPlayer; mLock_TargetPlayer.UnlockRead();  return player; }
	SPtr<GameMonster>			GetTargetMonster()			{ return mTargetMonster;  }
	SPtr_GameObject				GetPathTargetObject()		{ return mPathTarget;  }

	EnemyInfo::State			GetState()					{ return mState; }
	std::stack<Vec3>*			GetPaths()					{ return &mPaths; }
	FBProtocol::MONSTER_BT_TYPE GetMonsterCurrBTType()		{ return mCurrBTType; }
	FBProtocol::MONSTER_BT_TYPE GetMontserPrevBTType()		{ return mPrevBTType; }
	FBProtocol::MONSTER_BT_TYPE GetMonsterBTType() { mLock_BTType.LockRead(); FBProtocol::MONSTER_BT_TYPE btType = mBTType; mLock_BTType.UnlockRead(); return btType; }
	/// +---------------------------------------------------
	///						S E T T E R 
	/// ---------------------------------------------------+
	void SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE type) { mCurrBTType    = type; }
	void UpdateMonsterCurrBTType()								{ mPrevBTType    = mCurrBTType; }
	void SetState(EnemyInfo::State state)						{ mState         = state; }

	void SetPathTargetObject(SPtr<GameObject> target)			{ mPathTarget    = target; }
	void SetTargetPlayer(SPtr<GamePlayer> target) { mLock_TargetPlayer.LockWrite(); mTargetPlayer = target; mLock_TargetPlayer.UnlockWrite(); }
	void SetTargetMonster(SPtr<GameMonster> target)				{ mTargetMonster = target; }
	void SetOwnerMonster(SPtr<GameMonster> ownerMonster)		{ mOwnerMonster  = ownerMonster; }

	void SetBTType(FBProtocol::MONSTER_BT_TYPE btType) { mLock_BTType.LockWrite(); mBTType = btType; mLock_BTType.UnlockWrite(); }

	/// +---------------------------------------------------
	///						MIND CONTROL 
	/// ---------------------------------------------------+
	void OnMindControl()	{ mMindControllded  = true; }
	void OffMindControl()	{ mMindControllded  = false; }
	bool IsMindControlled() { return mMindControllded; }

};

