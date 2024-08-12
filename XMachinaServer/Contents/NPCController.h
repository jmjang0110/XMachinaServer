#pragma once
#include "GameMonster.h"
#include "GameBullet.h"
#include "GameNPC.h"
#include "GameRoom.h"
#include "GameItem.h"

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
	std::unordered_map<UINT32, SPtr<GameMonster>>	mMonsters;
	std::unordered_map<UINT32, SPtr<GameBullet>>	mBullets;
	std::unordered_map<UINT32, SPtr<GameNPC>>		mNPCs;

	std::unordered_map<UINT32, SPtr<GameItem>>		mStaticItems;	// Client Scene�� ����ȭ ( �̹� ������� )
	std::unordered_map<UINT32, SPtr<GameItem>>		mDynamicItems;  // Server ������ ����  

public:
	NPCController();
	~NPCController();

public:
	void Init(SPtr_GameRoom owner);

	/* sectorIdx �� ���� �ٸ� ���� ������ ���� - monTypes�� �ִ� MonsterType ���� ����*/
	void InitMonsters(Coordinate maxSectorIdx);
	void InitItems();
	void InitBullets();
	void InitNPCs();

	SPtr<GameMonster> CreateMonster(UINT32 id, Coordinate sectorIdx, MonsterType monType);
	void AddMonster(UINT32 id, SPtr<GameMonster> monster);
	void AddDynamicItem(UINT32 id, SPtr<GameItem> item);
	void AddStaticItem(UINT32 id, SPtr<GameItem> item);

	SPtr<GameRoom>  GetOwnerRoom() { return mOwnerRoom; }

	SPtr<GameMonster> GetMonster(UINT32 monsterID);
	SPtr<GameItem> GetStaticItem(UINT32 item_id);
	SPtr<GameItem> GetDynamicItem(UINT32 item_id);
	SPtr<GameItem> GetItem(UINT32 item_id);


	std::vector<SPtr<GameMonster>> GetMonstersInViewRange(Vec3 palyer_pos, float viewRange_radius);

};

