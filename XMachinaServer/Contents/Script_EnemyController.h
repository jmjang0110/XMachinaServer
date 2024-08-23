#pragma once

#include "Script.h"
/// +-------------------------------
///		     Script_EnemyController 
/// ________________________________
///	> [ ���� ] 
///		- Enemy Controller Script�� ���� �ִ� ��ü�� 
///		- �ڽ��� Enemy �̸� 
/// ________________________________
class NPCController;
class GameRoom;
class GameObject;
class Script_EnemyController : public Script
{
private:
	NPCController*		mOwnerNPCController = nullptr;
	GameRoom*			mOwnerRoom          = nullptr;
private:
	bool				mIsMindControlled  = {};

	SPtr<GameObject>	mTarget             = {}; Lock::SRWLock Lock_Target;
	SPtr<GameObject>	mInvoker			= {};

	SPtr<GameObject>	mPathTarget			= {};
	std::stack<Vec3>	mPaths				= {};

	// BTType�� �ٲܰ����� üũ�ϱ� ���� ������ (���¸� ��� �������� �ʰ� �ٲ���� ���� ���� - Lock �� �� �ּ�ȭ )
	FBProtocol::MONSTER_BT_TYPE mCurrBTType = FBProtocol::MONSTER_BT_TYPE::MONSTER_BT_TYPE_IDLE;
	FBProtocol::MONSTER_BT_TYPE mPrevBTType = FBProtocol::MONSTER_BT_TYPE::MONSTER_BT_TYPE_IDLE;
	
	/* Lock */
	FBProtocol::MONSTER_BT_TYPE mBTType = FBProtocol::MONSTER_BT_TYPE::MONSTER_BT_TYPE_IDLE; Lock::SRWLock mLock_BTType;

public:
	Script_EnemyController();
	Script_EnemyController(SPtr<GameObject> owner);
	virtual ~Script_EnemyController();

public:

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

public:
	void Reset();
	void RemoveAllAnimation();
	void ForceSetTarget(SPtr<GameObject> target) { mTarget = target; }

public:
	/// +---------------------------------------------------
	///						G E T T E R  
	/// ---------------------------------------------------+
	SPtr<GameObject>			GetPathTargetObject()			{ return mPathTarget;  }

	std::stack<Vec3>*			GetPaths()						{ return &mPaths; }
	FBProtocol::MONSTER_BT_TYPE GetMonsterCurrBTType()			{ return mCurrBTType; }
	FBProtocol::MONSTER_BT_TYPE GetMontserPrevBTType()			{ return mPrevBTType; }

	SPtr<GameObject>			GetTarget()						{ Lock_Target.LockRead(); SPtr<GameObject> target = mTarget; Lock_Target.UnlockRead(); return target; }
	FBProtocol::MONSTER_BT_TYPE GetMonsterBTType()				{ mLock_BTType.LockRead(); FBProtocol::MONSTER_BT_TYPE btType = mBTType; mLock_BTType.UnlockRead(); return btType; }
	
	NPCController*				GetOwnerNPCController()			{ return mOwnerNPCController; }
	GameRoom*					GetOwnerRoom()					{ return mOwnerRoom; }
	SPtr<GameObject>			GetInvoker()					{ return mInvoker; }
	
	/// +---------------------------------------------------
	///						S E T T E R 
	/// ---------------------------------------------------+
	void SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE type) { mPrevBTType    = mCurrBTType; mCurrBTType = type; }
	void UpdateMonsterCurrBTType()								{ mPrevBTType    = mCurrBTType; }

	void SetPathTargetObject(SPtr<GameObject> target)			{ mPathTarget    = target; }
	void SetInvoker(SPtr<GameObject> invoker)					{ mInvoker       = invoker; }
	
	void SetTarget(SPtr<GameObject> target)						{ Lock_Target.LockWrite(); mTarget  = target; Lock_Target.UnlockWrite(); }
	void SetBTType(FBProtocol::MONSTER_BT_TYPE btType)			{ mLock_BTType.LockWrite(); mBTType = btType; mLock_BTType.UnlockWrite(); }


	/// ----------------- Mind Control -----------------
	void OnMindControl()	{ mIsMindControlled = true; }
	void OffMindControl()	{ mIsMindControlled = false; }
	bool IsMindControlled() { return mIsMindControlled; }
};

