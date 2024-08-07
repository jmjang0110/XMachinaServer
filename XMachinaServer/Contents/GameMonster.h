#pragma once

#undef max
#include "Enum_generated.h"

#include "GameObject.h"
#include "DB_Monster.h"
#include "ObjectSnapShot.h"
#include "GamePhero.h"
#include "Script_EnemyStat.h"
#include "Script_Stat.h"

/// +-------------------------------
///		     Game Monster
/// ________________________________
///
/// Monster
///    ��
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
	FBProtocol::MONSTER_TYPE			Type;		/*	���� ����	*/

	/// +-----------------------------------------------------------
	///		Stat  
	/// -----------------------------------------------------------+
	std::string							Pheros;
};

class GameMonster : public GameObject
{
private:
	NPCController*			mOwnerNC;

	Coordinate				mSectorIndex;				// ��� ���Ϳ� �����ִ°�?
	MonsterSnapShot			mInfo; 
	
	std::atomic_int			mActivate_Ref = 0;

private:
	double					mTimer			 = 0.f;
											 
	Vec3					mSpawnPos		 = {};
	Vec3					mSpawnRot		 = {};
	
	Script_EnemyController* mEnemyController = nullptr;
	Script_EnemyStat*		mEnemyStat       = nullptr; // HP, IsDead
	int						HitCnt           = 0;

	std::atomic_bool mSendDeadMonsterPkt = false;




public:
	virtual SPtr<GameMonster> Clone();

	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


public:
	void DecreaseRef() { mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref.store(0); }
	void UpdateSnapShot(); // �ֽ� ���·� ������ ������Ʈ 

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
	const std::vector<SPtr<GamePhero>>& GetAllPheros() ;

	// Snap Shot ( in Script )
	Script_Stat::ObjectState	S_GetObjectState() { return mEnemyStat->S_GetObjectState(); } // Lock
	float	S_GetHp()		{ return mEnemyStat->S_GetHp();  } // Lock
	float	GetAttack()		{ return mEnemyStat->GetStat_AttackRate(); }
public:
	GameMonster();
	GameMonster(UINT32 id, Coordinate sectorIdx); /* Monster ���� ���̵� - (�����ǰ� �Ҹ�� �� ���� �ӽ� ���̵�)*/
	virtual ~GameMonster();
	 
};

