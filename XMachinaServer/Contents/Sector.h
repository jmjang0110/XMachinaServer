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
/// -------------------------------+


class Sector : public GameEntity
{
private:
	/* Player , NPC - � ID �� �ش� Sector�� �����ִ��� �����Ѵ�. ���� ��ü�� PlayerController �� NPCController�� ������ ���⿡ �ִ� ID�� ���ؼ� �����Ѵ�. */
	
	std::vector<int> mMonsters_IDInfos;	// �ش� Sector�� �ִ� Monster ID �� ����Ǿ� �ִ�. ( ���Ͱ� ������ IsActive�� ��� ���ٰ� ��Ȱ ��Ű�� true�� �ϴ� ��� )
	std::vector<int> mPlayers_IDInfos;	// �ش� Sector�� �����ִ� Player ID �̴�. 

public:
	Sector();
	Sector(int id);
	virtual ~Sector() override;

public:


};

