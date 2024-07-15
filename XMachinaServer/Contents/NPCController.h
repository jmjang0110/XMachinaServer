#pragma once
#include "GameMonster.h"
#include "GameBullet.h"
#include "GameNPC.h"
#include "GameRoom.h"

/// +-------------------------------
///		   NPCController
/// ________________________________
///	> [ ���� ]
/// - NPC, GameMonster, GameBullet ���� �����Ѵ�. 
/// - Player ������ PlayerController �� �ִ�. 
/// ________________________________
/// 
/// -------------------------------+

class NPCController
{
private:
	SPtr<GameRoom> mOwnerRoom;

private:
	/* Room ���� ��� GameObject (NPC) ���� */
	std::unordered_map<UINT32, SPtr<GameMonster>> mMonsters;
	std::unordered_map<UINT32, SPtr<GameBullet>>  mBullets;
	std::unordered_map<UINT32, SPtr<GameNPC>>     mNPCs;
	

public:
	NPCController();
	~NPCController();

public:
	void Init(SPtr_GameRoom owner);

	/* sectorIdx �� ���� �ٸ� ���� ������ ���� - monTypes�� �ִ� MonsterType ���� ����*/
	void InitMonsters(Coordinate maxSectorIdx);
	void InitBullets();
	void InitNPCs();

	SPtr<GameMonster> CreateMonster(UINT32 id, Coordinate sectorIdx, MonsterType monType);
	void AddMonster(UINT32 id, SPtr<GameMonster> monster);
	SPtr<GameRoom>  GetOwnerRoom() { return mOwnerRoom; }


	std::vector<SPtr<GameMonster>> GetMonstersInViewRange(Vec3 palyer_pos, float viewRange_radius);

};

