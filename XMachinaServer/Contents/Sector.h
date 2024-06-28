#pragma once

#include "GameEntity.h"
#include "GameMonster.h"
#include "GamePlayer.h"


/// +-------------------------------
///		        Sector
/// ________________________________
///
/// ������ ���� Sector �� ������.
/// Sector Controller �� �̸� �����Ѵ�.
/// 
/// 24.06.22 : � Player ���� ���� �ִ��� ��������.!! 
/// 24.06.27 : Player , NPC - � ID �� �ش� Sector�� �����ִ��� �����Ѵ�. ���� ��ü�� PlayerController �� NPCController�� ������ ���⿡ �ִ� ID�� ���ؼ� �����Ѵ�. 
/// -------------------------------+


class Sector : public GameEntity
{
private:
	std::vector<int> mMonsters_IDInfos;		// �ش� Sector�� �����ִ� Monster ID. ( ���Ͱ� ������ IsActive�� ��� ���ٰ� ��Ȱ ��Ű�� true�� �ϴ� ��� )
	std::vector<int> mPlayers_IDInfos;		// �ش� Sector�� �����ִ� Player ID. 
	std::vector<int> mNPCs_IDInfos;			// �ش� Secto�� �����ִ� NPC ID


public:
	Sector();
	Sector(int id);
	virtual ~Sector() override;

public:
	void Init();


};

