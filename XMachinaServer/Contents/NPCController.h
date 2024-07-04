#pragma once

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
	SPtr_GameRoom mOwnerRoom;

private:
	//concurrency::concurrent_unordered_map<UINT32, SPtr_GameStructure>	mStructures;			// Key : ID / Value : Structre ( Shared Ptr )
	//concurrency::concurrent_unordered_map<UINT32, SPtr_GameBullet>		mBullets;
	//concurrency::concurrent_unordered_map<UINT32, SPtr_GameMonster>		mMonsters;				// Key : ID / Value : Moster ( Shared Ptr )

	/* 배열로 관리하는 것이 나을 것 같은데?? */
	

public:
	NPCController();
	~NPCController();

public:
	void Init(SPtr_GameRoom owner);


};

