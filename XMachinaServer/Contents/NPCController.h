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

struct StructureInfos
{
	// Sector���� ��� Structure ����
	concurrency::concurrent_unordered_map<UINT32, SPtr_GameStructure>	Structures;			// Key : ID / Value : Structre ( Shared Ptr )

};

struct BulletInfos
{
	// Sector���� ��� Bullet ����
	concurrency::concurrent_unordered_map<UINT32, SPtr_GameBullet> Bullets;


};

struct MonsterInfos
{
	// Secotr���� ��� Monster ���� 
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

