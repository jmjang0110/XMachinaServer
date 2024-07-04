#pragma once

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
	SPtr_GameRoom mOwnerRoom;

private:
	//concurrency::concurrent_unordered_map<UINT32, SPtr_GameStructure>	mStructures;			// Key : ID / Value : Structre ( Shared Ptr )
	//concurrency::concurrent_unordered_map<UINT32, SPtr_GameBullet>		mBullets;
	//concurrency::concurrent_unordered_map<UINT32, SPtr_GameMonster>		mMonsters;				// Key : ID / Value : Moster ( Shared Ptr )

	/* �迭�� �����ϴ� ���� ���� �� ������?? */
	

public:
	NPCController();
	~NPCController();

public:
	void Init(SPtr_GameRoom owner);


};

