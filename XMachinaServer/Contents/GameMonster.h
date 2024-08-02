#pragma once

#undef max
#include "Enum_generated.h"

#include "GameObject.h"
#include "DB_Monster.h"
#include "ObjectSnapShot.h"
#include "GamePhero.h"
#include "Script_EnemyStat.h"

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
class Script_EnemyStat;

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
	std::string							Pheros;
};

class GameMonster : public GameObject
{
private:
	NPCController*			mOwnerNC;

	Coordinate				mSectorIndex;				// 어느 섹터에 속해있는가?
	MonsterSnapShot			mInfo; 
	
	std::atomic_int			mActivate_Ref = 0;

	double					mTimer = 0.f;

	Vec3					mSpawnPos;
	Vec3					mSpawnRot;
	
	Script_EnemyController* mEnemyController = nullptr;
	Script_EnemyStat*		mEnemyStat       = nullptr; // HP, IsDead
	int						HitCnt           = 0;

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
	void SetMonsterID(uint32_t id)									{ mInfo.ID         = id; }
	void SetMonsterType(FBProtocol::MONSTER_TYPE type)				{ mInfo.Type       = type; }
	void SetSectorIndex(Coordinate sectorIdx)						{ mSectorIndex     = sectorIdx; }
	void SetPheros(std::string pheros)								{ mInfo.Pheros     = pheros; }
	void SetOwnerNPCController(NPCController* nc)					{ mOwnerNC         = nc; }
	void SetEnemyController(Script_EnemyController* script)			{ mEnemyController = script; }
	void SetEnemyStat(Script_EnemyStat* script)						{ mEnemyStat       = script; }

	void SetSNS_IsDead(bool isdead)									{ mEnemyStat->SetSNS_IsDead(isdead); } // lock
	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+	
	SPtr<GameMonster>					 GetSnapShotOwner()			{ return mInfo.owner;		}
	uint32_t							 GetMonsterID()				{ return mInfo.ID;			}
	FBProtocol::MONSTER_TYPE			 GetMonsterType()			{ return mInfo.Type;		}
	std::string							 GetPheros()				{ return mInfo.Pheros;		}
	NPCController*						 GetOwnerNPCController()	{ return mOwnerNC; }
	Script_EnemyController*				 GetEnemyController()		{ return mEnemyController; }
	Script_EnemyStat*					 GetEnemyStat()				{ return mEnemyStat; }
	int									 GetActivate_RefCnt()		{ return mActivate_Ref.load(); }
	const std::vector<SPtr<GameObject>>& GetAllPheros() ;

	// Snap Shot ( in Script )
	bool	GetSNS_IsDead() { return mEnemyStat->GetSNS_IsDead();  } // Lock
	float	GetSNS_HP()		{ return mEnemyStat->GetSNS_HP();  } // Lock
	float	GetAttack()		{ return mEnemyStat->GetStat_AttackRate(); }
public:
	GameMonster();
	GameMonster(UINT32 id, Coordinate sectorIdx); /* Monster 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	virtual ~GameMonster();
	 
};

