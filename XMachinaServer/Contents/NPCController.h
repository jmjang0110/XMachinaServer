#pragma once
#include "GameMonster.h"
#include "GameBullet.h"
#include "GameNPC.h"
#include "GameRoom.h"

/// +-------------------------------
///		   NPCController
/// ________________________________
///	> [ 설명 ]
/// - NPC, GameMonster, GameBullet 들을 관리한다. 
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
	std::unordered_map<UINT32, SPtr<GameMonster>> mMonsters;
	std::unordered_map<UINT32, SPtr<GameBullet>>  mBullets;
	std::unordered_map<UINT32, SPtr<GameNPC>>     mNPCs;
	

public:
	NPCController();
	~NPCController();

public:
	void Init(SPtr_GameRoom owner);

	/* sectorIdx 에 따라서 다른 몬스터 종류를 생성 - monTypes에 있는 MonsterType 몬스터 생성*/
	void InitMonsters(Coordinate maxSectorIdx);
	void InitBullets();
	void InitNPCs();

	SPtr<GameMonster> CreateMonster(UINT32 id, Coordinate sectorIdx, MonsterType monType);
	void AddMonster(UINT32 id, SPtr<GameMonster> monster);
	SPtr<GameRoom>  GetOwnerRoom() { return mOwnerRoom; }


	std::vector<SPtr<GameMonster>> GetMonstersInViewRange(Vec3 palyer_pos, float viewRange_radius);

};

