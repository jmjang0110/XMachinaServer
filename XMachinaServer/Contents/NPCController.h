#pragma once

/// +-------------------------------
///		   NPCController
/// ________________________________
///	> [ ���� ]
/// - NPC, GameMonster, GameBullet ���� �����Ѵ�. 
/// ________________________________
/// 
/// -------------------------------+

struct StructureInfos
{

};

struct BulletInfos
{

};

struct MonsterInfos
{

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

