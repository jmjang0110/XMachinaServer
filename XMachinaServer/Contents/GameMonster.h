#pragma once

#undef max
#include "Enum_generated.h"

#include "GameObject.h"
#include "DB_Monster.h"
#include "ObjectSnapShot.h"
#include "GamePhero.h"

/// +-------------------------------
///		     Game Monster
/// ________________________________
///
/// Monster
///    ↑
///	Monster_Ursacetus
///	Monster_Onyscidus
///	Monster_AdvancedCombatDroid_5
/// -------------------------------+

class NPCController;

struct MonsterSnapShot : public ObjectSnapShot
{
	/// +-----------------------------------------------------------
	///		base 
	/// -----------------------------------------------------------+
	SPtr<GameMonster>					owner = nullptr;
	FBProtocol::MONSTER_TYPE			Type;		/*	몬스터 종류	*/
	FBProtocol::MONSTER_BT_TYPE			CurrState;  Lock::SRWLock lock_CurrState;

	/// +-----------------------------------------------------------
	///		Stat  
	/// -----------------------------------------------------------+
	float								HP;			Lock::SRWLock lock_HP;/*		HP		*/
	float								Attack;		Lock::SRWLock lock_Attack;/*	  공격력		*/
	std::string							Pheros;

};

class GameMonster : public GameObject
{
private:
	NPCController*	mOwnerNC;

	Coordinate		mSectorIndex;   // 어느 섹터에 속해있는가?
	Lock::SRWLock	mLock_SnapShot;
	MonsterSnapShot mInfo;			// 몬스터 정보 
	
	std::atomic_int mActivate_Ref = 0;

	double		mTimer = 0.f;

	Vec3		mSpawnPos;
	Vec3		mSpawnRot;

public:
	virtual SPtr<GameMonster> Clone();

	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


public:
	void DecreaseRef() { mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }
	void UpdateSnapShot(); // 최신 상태로 스냅샷 업데이트 

	void Broadcast_SPkt_Monster_Transform();
	void Broadcast_SPkt_Mosnter_State(FBProtocol::MONSTER_BT_TYPE monster_bt_type);
	void Send_SPkt_Mosnter_State(FBProtocol::MONSTER_BT_TYPE monser_bt_type);

	void On_ExitFromViewList();


public:
	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+
	void SetMonsterID(uint32_t id)							{ mInfo.ID			                                = id;}
	void SetMonsterType(FBProtocol::MONSTER_TYPE type)		{ mInfo.Type		                                = type;}
	void SetAttack(float attack)							{ mInfo.Attack		                                = attack; }
	void SetSectorIndex(Coordinate sectorIdx)				{ mSectorIndex                                      = sectorIdx; }
	void SetPheros(std::string pheros)						{ mInfo.Pheros                                      = pheros; }

	void SetHP(float hp)									{ mInfo.lock_HP.LockWrite(); mInfo.HP               = hp;	mInfo.lock_HP.UnlockWrite(); }
	void SetBTState(FBProtocol::MONSTER_BT_TYPE type)		{ mInfo.lock_CurrState.LockWrite(); mInfo.CurrState = type; mInfo.lock_CurrState.UnlockWrite(); }

	void SetOwnerNPCController(NPCController* nc)			{ mOwnerNC = nc; }
	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+	
	SPtr<GameMonster>					 GetSnapShotOwner()			{ return mInfo.owner;		}
	uint32_t							 GetMonsterID()				{ return mInfo.ID;			}
	FBProtocol::MONSTER_TYPE			 GetMonsterType()			{ return mInfo.Type;		}
	float								 GetAttack()				{ return mInfo.Attack;		}
	std::string							 GetPheros()				{ return mInfo.Pheros;		}

	float								 GetHP()					{ mInfo.lock_HP.LockRead();  float hp = mInfo.HP;	mInfo.lock_HP.UnlockRead(); return hp; }
	FBProtocol::MONSTER_BT_TYPE			 GetBTState()				{ mInfo.lock_CurrState.LockRead(); FBProtocol::MONSTER_BT_TYPE state = mInfo.CurrState; mInfo.lock_CurrState.UnlockRead(); return state; }
	
	NPCController*						 GetOwnerNPCController()	{ return mOwnerNC; }
	int									 GetActivate_RefCnt()				{ return mActivate_Ref.load(); }
	const std::vector<SPtr<GameObject>>& GetAllPheros() ;


public:
	GameMonster();
	GameMonster(UINT32 id, Coordinate sectorIdx); /* Monster 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	virtual ~GameMonster();
	 
};

