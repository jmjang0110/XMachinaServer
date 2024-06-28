#pragma once

/// +-------------------------------
///		   NPCController
/// ________________________________
///	> [ 설명 ]
/// - NPC, GameMonster, GameBullet 들을 관리한다. 
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

