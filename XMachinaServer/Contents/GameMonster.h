#pragma once

#include "GameObject.h"
#include "DB_Monster.h"

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

struct MonsterSnapShot 
{
	uint32_t			ID;
	MonsterType			Type;		/*	몬스터 종류	*/
	
	/* Stat Script 로 빼자... */
	std::atomic<float>  HP;			/*		HP		*/
	float				Attack;		/*	  공격력		*/

	/* ... Transform Component로 빼자 */
	Atomic_Vec3				Position;
	Atomic_Vec3				Rotation;
	Atomic_Vec3				SpineDir;
};

struct MonsterSnapShot_ReadOnly // 외부에서 MonsterSnapSHot 을 읽어서 복사해 저장할 때 사용 .
{
	uint32_t			ID;
	MonsterType			Type;		/*	몬스터 종류	*/

	/* Stat Script 로 빼자... */
	float				HP;			/*		HP		*/
	float				Attack;		/*	  공격력		*/

	/* ... Transform Component로 빼자 */
	Vec3				Position;
	Vec3				Rotation;
	Vec3				SpineDir;

};

class GameMonster : public GameObject
{
private:
	NPCController* mOwnerNC;

	Coordinate  mSectorIndex;   // 어느 섹터에 속해있는가?
	MonsterSnapShot mInfo;			// 몬스터 정보 
	
	std::atomic_int mActivate_Ref = 0;


public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

public:
	void SetID(uint32_t id)					{ mInfo.ID = id; }
	void SetType(MonsterType type)			{ mInfo.Type = type; }
	void SetHP(float hp)					{ mInfo.HP.store(hp); }
	void SetAttack(float attack)			{ mInfo.Attack   = attack; }
	void SetPosition(Vec3 position)			{ mInfo.Position.x.store(position.x); mInfo.Position.y.store(position.y); mInfo.Position.z.store(position.z); }
	void SetRotation(Vec3 rotation)			{ mInfo.Rotation.x.store(rotation.x); mInfo.Rotation.y.store(rotation.y); mInfo.Rotation.z.store(rotation.z); }
	void SetSpineDir(Vec3 spineDir)			{ mInfo.SpineDir.x.store(spineDir.x); mInfo.SpineDir.y.store(spineDir.y); mInfo.SpineDir.z.store(spineDir.z); }

	// Get 함수들
	uint32_t	GetID()			const { return mInfo.ID; }
	MonsterType GetType()		const { return mInfo.Type; }
	float		GetHP()			const { return mInfo.HP.load(); }
	float		GetAttack()		const { return mInfo.Attack; }
	Vec3		GetPosition()	const { return Vec3{ mInfo.Position.x.load(), mInfo.Position.y.load(), mInfo.Position.z.load() };}
	Vec3		GetRotation()	const {	return Vec3{ mInfo.Rotation.x.load(), mInfo.Rotation.y.load(), mInfo.Rotation.z.load() };}
	Vec3		GetSpineDir() const {	return Vec3{ mInfo.SpineDir.x.load(), mInfo.SpineDir.y.load(), mInfo.SpineDir.z.load() };}

	void SetOwnerNPCController(NPCController* nc) { mOwnerNC = nc; }
	NPCController* GetOwnerNPCController() { return mOwnerNC; }
	int GetActivate_RefCnt() { return mActivate_Ref.load(); }
	void DecreaseRef() { mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }

	MonsterSnapShot_ReadOnly GetCopySnapShot();


public:
	GameMonster();
	GameMonster(UINT32 id, Coordinate sectorIdx); /* Monster 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	virtual ~GameMonster();

};

