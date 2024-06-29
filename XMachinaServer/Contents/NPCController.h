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

struct StructureInfos
{
	// Sector내에 모든 Structure 정보
	concurrency::concurrent_unordered_map<UINT32, SPtr_GameStructure>	Structures;			// Key : ID / Value : Structre ( Shared Ptr )

};

struct BulletInfos
{
	// Sector내에 모든 Bullet 정보
	concurrency::concurrent_unordered_map<UINT32, SPtr_GameBullet> Bullets;


};

struct MonsterInfos
{
	// Secotr내에 모든 Monster 정보 
	concurrency::concurrent_unordered_map<UINT32, SPtr_GameMonster>	Monsters;			// Key : ID / Value : Moster ( Shared Ptr )

};

class NPCController
{
private:
	SPtr_GameRoom mOwnerRoom;

private:
	MonsterInfos	mMI;
	BulletInfos		mBI;
	StructureInfos	mSI;

public:
	NPCController();
	~NPCController();

public:
	void Init(SPtr_GameRoom owner);


};

