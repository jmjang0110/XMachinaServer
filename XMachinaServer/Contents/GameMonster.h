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
	SPtr<GameMonster>					owner = nullptr;
	FBProtocol::MONSTER_TYPE			Type;		/*	몬스터 종류	*/
	FBProtocol::MONSTER_BT_TYPE			CurrState;  Lock::SRWLock lock_CurrState;
	/* Stat Script 로 빼자... */
	float								HP;			Lock::SRWLock lock_HP;/*		HP		*/
	float								Attack;		Lock::SRWLock lock_Attack;/*	  공격력		*/

	Vec3								Position;	Lock::SRWLock lock_Position;
	Vec3								Rotation;	Lock::SRWLock lock_Rotation;
	Vec3								SpineDir;	Lock::SRWLock lock_SpineDir;

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

	double mTimer = 0.f;

public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

	virtual SPtr<GameMonster> Clone();

public:
	void SetMonsterID(uint32_t id)							{ mInfo.ID			= id;}
	void SetMonsterType(FBProtocol::MONSTER_TYPE type)		{ mInfo.Type		= type;}
	void SetAttack(float attack)							{ mInfo.Attack		= attack; }
		
	void SetHP(float hp)									{ mInfo.lock_HP.LockWrite();		mInfo.HP               = hp;			mInfo.lock_HP.UnlockWrite(); }
	void SetPosition(Vec3 pos)								{ mInfo.lock_Position.LockWrite();	mInfo.Position         = pos;			mInfo.lock_Position.UnlockWrite(); }
	void SetRotation(Vec3 rot)								{ mInfo.lock_Rotation.LockWrite();	mInfo.Rotation         = rot;			mInfo.lock_Rotation.UnlockWrite(); }
	void SetSpineDir(Vec3 spinedir)							{ mInfo.lock_SpineDir.LockWrite();	mInfo.SpineDir         = spinedir;		mInfo.lock_SpineDir.UnlockWrite(); }

	void SetSectorIndex(Coordinate sectorIdx);
	void SetPheros(std::string pheros)						{ mInfo.Pheros = pheros; }
	void SetBTState(FBProtocol::MONSTER_BT_TYPE type)		{ mInfo.lock_CurrState.LockWrite(); mInfo.CurrState = type; mInfo.lock_CurrState.UnlockWrite(); }
	// Get 함수들
	SPtr<GameMonster>			GetSnapShotOwner()			{	 mInfo.owner; }
	uint32_t					GetMonsterID()				{	return mInfo.ID;		}
	FBProtocol::MONSTER_TYPE	GetMonsterType()			{	return mInfo.Type;		}
	float						GetAttack()					{	return mInfo.Attack;	}
	float						GetHP()						{ mInfo.lock_HP.LockRead();  float hp = mInfo.HP;	mInfo.lock_HP.UnlockRead(); return hp; }
	Vec3						GetPosition()				{ mInfo.lock_Position.LockRead(); Vec3 pos = mInfo.Position; mInfo.lock_Position.UnlockRead(); return pos; }
	FBProtocol::MONSTER_BT_TYPE GetBTState() { mInfo.lock_CurrState.LockRead(); FBProtocol::MONSTER_BT_TYPE state = mInfo.CurrState; mInfo.lock_CurrState.UnlockRead(); return state; }
	std::string					GetPheros()					{ return mInfo.Pheros; }
	// mPheros 벡터를 반환하는 getter 함수
	const std::vector<SPtr<GameObject>>& GetAllPheros() ;



	void						SetOwnerNPCController(NPCController* nc)	{ mOwnerNC = nc; }
	NPCController*				GetOwnerNPCController()						{ return mOwnerNC; }
	int							GetActivate_RefCnt()						{ return mActivate_Ref.load(); }
	void						DecreaseRef()								{ mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }

	MonsterSnapShot GetSnapShot() { mLock_SnapShot.LockWrite(); MonsterSnapShot snapShot = mInfo; mLock_SnapShot.UnlockWrite(); return snapShot; }

	void UpdateSnapShot(); // 최신 상태로 스냅샷 업데이트 

	void On_ExitFromViewList();

	void Broadcast_SPkt_Monster_Transform();
	void Broadcast_SPkt_Mosnter_State(FBProtocol::MONSTER_BT_TYPE monster_bt_type);
	
	void Send_SPkt_Mosnter_State(FBProtocol::MONSTER_BT_TYPE monser_bt_type);


public:
	GameMonster();
	GameMonster(UINT32 id, Coordinate sectorIdx); /* Monster 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	virtual ~GameMonster();
	 
};

