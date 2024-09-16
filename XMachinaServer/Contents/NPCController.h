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

	/* Dropped Item In Battle Scene */
	std::unordered_map<UINT32, SPtr<GameObject>>		mDroppedItems; // Server Managing Dropped ITem ;

public:
	NPCController();
	~NPCController();

public:
	void Init(SPtr<GameRoom> owner);

	void InitMonsters();
	void InitItems();


	void AddMonster(UINT32 id, SPtr<GameObject> monster);
	void AddDynamicItem(UINT32 id, SPtr<GameObject> item);
	void AddStaticItem(UINT32 id, SPtr<GameObject> item);

	void AddDroppedITem(UINT32 id, SPtr<GameObject> item);


	SPtr<GameRoom>			GetOwnerRoom() { return mOwnerRoom; }

	SPtr<GameObject>		GetMonster(UINT32 monsterID);
	SPtr<GameObject>		GetStaticItem(UINT32 item_id);
	SPtr<GameObject>		GetDynamicItem(UINT32 item_id);
	SPtr<GameObject>		GetItem(UINT32 item_id);

};

