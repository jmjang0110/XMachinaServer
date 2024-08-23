#pragma once
#include "GameRoom.h"
/// +-------------------------------
///		   NPCController
/// ________________________________
///	> [ 설명 ]
/// - NPC, GameObject, GameBullet 들을 관리한다. 
/// - Player 정보는 PlayerController 에 있다. 
/// ________________________________
/// 
/// -------------------------------+

class NPCController
{
private:
	SPtr<GameRoom> mOwnerRoom;

private:
	/* Room 안의 모든 GameObject (NPC) 관리 */
	std::unordered_map<UINT32, SPtr<GameObject>>		mMonsters;
	std::unordered_map<UINT32, SPtr<GameObject>>		mNPCs;

	std::unordered_map<UINT32, SPtr<GameObject>>		mStaticItems;	// Client Scene과 동기화 ( 이미 만들어짐 )
	std::unordered_map<UINT32, SPtr<GameObject>>		mDynamicItems;  // Server 에서만 관리  

public:
	NPCController();
	~NPCController();

public:
	void Init(SPtr<GameRoom> owner);

	/* sectorIdx 에 따라서 다른 몬스터 종류를 생성 - monTypes에 있는 MonsterType 몬스터 생성*/
	void InitMonsters(Coordinate maxSectorIdx);
	void InitItems();


	void AddMonster(UINT32 id, SPtr<GameObject> monster);
	void AddDynamicItem(UINT32 id, SPtr<GameObject> item);
	void AddStaticItem(UINT32 id, SPtr<GameObject> item);
	void AddBuilding(UINT32 id, SPtr<GameObject> buildings);


	SPtr<GameRoom>			GetOwnerRoom() { return mOwnerRoom; }

	SPtr<GameObject>		GetMonster(UINT32 monsterID);
	SPtr<GameObject>		GetStaticItem(UINT32 item_id);
	SPtr<GameObject>		GetDynamicItem(UINT32 item_id);
	SPtr<GameObject>		GetItem(UINT32 item_id);


	std::vector<SPtr<GameObject>> GetMonstersInViewRange(Vec3 palyer_pos, float viewRange_radius);

};

