#pragma once

#include "GameEntity.h"
#include "GameMonster.h"
#include "GamePlayer.h"


/// +-------------------------------
///		        Sector
/// ________________________________
///
/// 지형을 여러 Sector 로 나눈다.
/// Sector Controller 가 이를 관리한다.
/// 
/// 24.06.22 : 어떤 Player 들이 들어와 있는지 관리하자.!! 
/// -------------------------------+


class Sector : public GameEntity
{
private:
	/* Player , NPC - 어떤 ID 가 해당 Sector에 속해있는지 관리한다. 실제 객체는 PlayerController 나 NPCController에 있으니 여기에 있는 ID를 통해서 접근한다. */
	
	std::vector<int> mMonsters_IDInfos;	// 해당 Sector에 있는 Monster ID 가 저장되어 있다. ( 몬스터가 죽으면 IsActive를 잠시 껐다가 부활 시키면 true로 하는 방식 )
	std::vector<int> mPlayers_IDInfos;	// 해당 Sector에 속해있는 Player ID 이다. 

public:
	Sector();
	Sector(int id);
	virtual ~Sector() override;

public:


};

