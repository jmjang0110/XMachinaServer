#pragma once

#include "SectorController.h"
#include "GameObject.h"
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

class Sector : public GameObject
{
private:
	SectorInfo::Type mType = SectorInfo::Type::None;
	Coordinate		 mIndex = {};
private:
	std::unordered_map<UINT32, SPtr<GameMonster>> mMonsters;
	std::unordered_map<UINT32, SPtr<GameNPC>>	  mNPCs;


public:
	Sector();
	Sector(UINT32 id, Coordinate sectorIdx); /* NPC ���� ���̵� - (�����ǰ� �Ҹ�� �� ���� �ӽ� ���̵�)*/
	virtual ~Sector();


public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


	/* �ʱ�ȭ - Room/NPCController�� Sector�� Monster�� �߰��Ѵ�. */
public:
	void Init(SectorInfo::Type type = SectorInfo::Type::None);

	bool AddMonster(UINT32 id, SPtr<GameMonster> monster);
	bool AddNPC(UINT32 id, SPtr<GameNPC> npc);


	SectorInfo::Type GetSectorType() { return mType; }


};

