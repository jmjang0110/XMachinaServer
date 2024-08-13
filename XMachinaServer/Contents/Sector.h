#pragma once

#include "SectorController.h"
#include "GameObject.h"
#include "GamePlayer.h"
#include "GameMonster.h"
#include "GameNPC.h"

/// +-------------------------------
///		        Sector
/// ________________________________
///
/// ������ ���� Sector �� ������.
/// Sector Controller �� �̸� �����Ѵ�.
/// 
/// 24.06.22 : � Player ���� ���� �ִ��� ��������.!! 
/// 24.06.27 : Player , NPC - � ID �� �ش� Sector�� �����ִ��� �����Ѵ�. ���� ��ü�� PlayerController �� NPCController�� ������ ���⿡ �ִ� ID�� ���ؼ� �����Ѵ�. 
/// -------------------------------+

class GameMonster;
class GameNPC;
class SectorController;

class Sector : public GameObject
{
private:
	SectorInfo::Type mType = SectorInfo::Type::None;
	Coordinate		 mIndex = {};
	SectorController* mOwnerSC = nullptr;

private:
	std::atomic<int> mActivateRef = 0;

	std::unordered_map<UINT32, SPtr<GameMonster>>		 mMonsters;
	std::unordered_map<UINT32, SPtr<GameNPC>>			 mNPCs;
	std::unordered_map<UINT32, SPtr<GameObject>>		 mBuildings; // ������ ( ex. ����.. ) 

public:
	Sector();
	Sector(UINT32 id, Coordinate sectorIdx); /* NPC ���� ���̵� - (�����ǰ� �Ҹ�� �� ���� �ӽ� ���̵�)*/
	virtual ~Sector();

public:
	void PrintInfo();


public:
	virtual void Activate();
	virtual void DeActivate();
	virtual bool IsActive();


	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;
	/* �ʱ�ȭ - Room/NPCController�� Sector�� Monster�� �߰��Ѵ�. */
public:
	void Init(Coordinate Index, SectorInfo::Type type = SectorInfo::Type::None);

	bool AddMonster(UINT32 id, SPtr<GameMonster> monster);
	bool AddNPC(UINT32 id, SPtr<GameNPC> npc);
	bool AddBuilding(UINT32 id, SPtr<GameObject> building);

	void RemoveMonster(UINT32 id);
	
public:
	/// +---------------------------------------------------------
	///	S E T T E R
	/// ---------------------------------------------------------+
	void SetOwnerSectorController(SectorController* ownerSC) { mOwnerSC = ownerSC; }


	/// +---------------------------------------------------------
	///	G E T T E R
	/// ---------------------------------------------------------+
	SectorInfo::Type				GetSectorType()					{ return mType; }
	SectorController*				GetOwnerSectorController()		{ return mOwnerSC; }
	std::vector<SPtr<GameMonster>>	GetMonstersInViewRange(Vec3 player_pos, float viewRange_radius);

	/// +---------------------------------------------------------
	///	  C O L L I S I O N 
	/// ---------------------------------------------------------+
	float CollideCheckRay_MinimumDist(const Ray& ray, GameObjectInfo::Type targetType) ;



};

