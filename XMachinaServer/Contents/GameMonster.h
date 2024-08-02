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
class Script_EnemyController;

struct MonsterSnapShot : public ObjectSnapShot
{
	/// +-----------------------------------------------------------
	///		base 
	/// -----------------------------------------------------------+
	SPtr<GameMonster>					owner = nullptr;
	FBProtocol::MONSTER_TYPE			Type;		/*	몬스터 종류	*/

	/// +-----------------------------------------------------------
	///		Stat  
	/// -----------------------------------------------------------+
	float								HP;			Lock::SRWLock lock_HP;	/*		HP		*/
	float								Attack;								/*	  공격력		*/
	std::string							Pheros;
	bool	IsDead = false; Lock::SRWLock lock_IsDead;
};

class GameMonster : public GameObject
{
private:
	NPCController*			mOwnerNC;

	Coordinate				mSectorIndex;							// 어느 섹터에 속해있는가?
	MonsterSnapShot			mInfo; Lock::SRWLock mLock_SnapShot;	
	
	std::atomic_int			mActivate_Ref = 0;

	double					mTimer = 0.f;

	Vec3					mSpawnPos;
	Vec3					mSpawnRot;
	
	Script_EnemyController* mEnemyController = nullptr;

	int HitCnt = 0;

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
	void OnHit();


public:
	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+
	void SetMonsterID(uint32_t id)							{ mInfo.ID			                                = id;}
	void SetMonsterType(FBProtocol::MONSTER_TYPE type)		{ mInfo.Type		                                = type;}
	void SetAttack(float attack)							{ mInfo.Attack		                                = attack; }
	void SetSectorIndex(Coordinate sectorIdx)				{ mSectorIndex                                      = sectorIdx; }
	void SetPheros(std::string pheros)						{ mInfo.Pheros                                      = pheros; }

	void SetSNS_HP(float hp)								{ mInfo.lock_HP.LockWrite(); mInfo.HP               = hp;	mInfo.lock_HP.UnlockWrite(); }
	void SetSNS_IsDead(bool isdead)							{ mInfo.lock_IsDead.LockWrite(); mInfo.IsDead = isdead;	mInfo.lock_IsDead.UnlockWrite(); }

	void SetOwnerNPCController(NPCController* nc)			{ mOwnerNC = nc; }
	void SetEnemyController(Script_EnemyController* script) { mEnemyController = script; }
	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+	
	SPtr<GameMonster>					 GetSnapShotOwner()			{ return mInfo.owner;		}
	uint32_t							 GetMonsterID()				{ return mInfo.ID;			}
	FBProtocol::MONSTER_TYPE			 GetMonsterType()			{ return mInfo.Type;		}
	float								 GetAttack()				{ return mInfo.Attack;		}
	std::string							 GetPheros()				{ return mInfo.Pheros;		}

	float								 GetSNS_HP()				{ mInfo.lock_HP.LockRead();  float hp = mInfo.HP;	mInfo.lock_HP.UnlockRead(); return hp; }
	bool								 GetSNS_IsDead()			{ mInfo.lock_IsDead.LockRead();  bool isdead = mInfo.IsDead;	mInfo.lock_IsDead.UnlockRead(); return isdead; }

	NPCController*						 GetOwnerNPCController()	{ return mOwnerNC; }
	Script_EnemyController*				 GetEnemyController()		{ return mEnemyController; }
	int									 GetActivate_RefCnt()		{ return mActivate_Ref.load(); }
	const std::vector<SPtr<GameObject>>& GetAllPheros() ;

public:
	GameMonster();
	GameMonster(UINT32 id, Coordinate sectorIdx); /* Monster 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	virtual ~GameMonster();
	 
};

