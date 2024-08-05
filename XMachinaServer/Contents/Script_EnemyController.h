#pragma once

#include "Script.h"
#include "GameObject.h"
#include "GamePlayer.h"
#include "GameMonster.h"
#include "Animation.h"

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
		Attack2,
		Attack3,
		Death,
		END,
	};
}

class NPCController;
class GameRoom;
class Script_EnemyController : public Script
{
private:
	SPtr<GameMonster>	mOwnerMonster;
	NPCController*		mOwnerNPCController = nullptr;
	GameRoom*			mOwnerRoom          = nullptr;
private:
	bool				mMindControllded	= false;
	EnemyInfo::State	mState				= EnemyInfo::State::Idle;

	SPtr<GameObject>	mTarget             = {}; Lock::SRWLock Lock_Target;
	SPtr_GameObject		mPathTarget			= {};
	std::stack<Vec3>	mPaths				= {};


	// BTType을 바꿀건지를 체크하기 위해 생성함 (상태를 계속 저장하지 않고 바뀌었을 떄만 저장 - Lock 빈도 수 최소화 )
	FBProtocol::MONSTER_BT_TYPE mCurrBTType = FBProtocol::MONSTER_BT_TYPE::MONSTER_BT_TYPE_IDLE;
	FBProtocol::MONSTER_BT_TYPE mPrevBTType = FBProtocol::MONSTER_BT_TYPE::MONSTER_BT_TYPE_IDLE;
	
	/* Lock */
	FBProtocol::MONSTER_BT_TYPE mBTType = FBProtocol::MONSTER_BT_TYPE::MONSTER_BT_TYPE_IDLE; Lock::SRWLock mLock_BTType;


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
	SPtr<GameMonster>			GetOwnerMonster()				{ return mOwnerMonster; }

	SPtr_GameObject				GetPathTargetObject()			{ return mPathTarget;  }

	EnemyInfo::State			GetState()						{ return mState; }
	std::stack<Vec3>*			GetPaths()						{ return &mPaths; }
	FBProtocol::MONSTER_BT_TYPE GetMonsterCurrBTType()			{ return mCurrBTType; }
	FBProtocol::MONSTER_BT_TYPE GetMontserPrevBTType()			{ return mPrevBTType; }

	SPtr<GameObject>			GetTarget()						{ Lock_Target.LockRead(); SPtr<GameObject> target = mTarget; Lock_Target.UnlockRead(); return target; }
	FBProtocol::MONSTER_BT_TYPE GetMonsterBTType()				{ mLock_BTType.LockRead(); FBProtocol::MONSTER_BT_TYPE btType = mBTType; mLock_BTType.UnlockRead(); return btType; }
	
	NPCController*				GetOwnerNPCController()			{ return mOwnerNPCController; }
	GameRoom*					GetOwnerRoom()					{ return mOwnerRoom; }

	
	/// +---------------------------------------------------
	///						S E T T E R 
	/// ---------------------------------------------------+
	void SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE type) { mCurrBTType    = type; }			
	void UpdateMonsterCurrBTType()								{ mPrevBTType    = mCurrBTType; }
	void SetState(EnemyInfo::State state)						{ mState         = state; }

	void SetPathTargetObject(SPtr<GameObject> target)			{ mPathTarget    = target; }
	void SetOwnerMonster(SPtr<GameMonster> ownerMonster);
	
	void SetTarget(SPtr<GameObject> target)						{ Lock_Target.LockWrite(); mTarget = target; Lock_Target.UnlockWrite(); }
	void SetBTType(FBProtocol::MONSTER_BT_TYPE btType)			{ mLock_BTType.LockWrite(); mBTType = btType; mLock_BTType.UnlockWrite(); }

	/// +---------------------------------------------------
	///						MIND CONTROL 
	/// ---------------------------------------------------+
	void OnMindControl()										{ mMindControllded  = true; }
	void OffMindControl()										{ mMindControllded  = false; }
	bool IsMindControlled()										{ return mMindControllded; }
};

