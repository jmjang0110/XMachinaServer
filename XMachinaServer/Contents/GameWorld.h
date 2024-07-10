#pragma once


/// +-------------------------------
///		     GameWorld
/// ________________________________
///
///	서버에서 현재 Game 상태를 전부 관리한다.
/// Game World의 상태를 업데이트하고 저장하는 역할이다.
/// 
/// GameWorld가 업데이트 되면 업데이트 정보를 
/// 클라이언트들에게 패킷을 보내 서버의 GameWorld를 동기화한다.

/// -------------------------------+

#include "Monster_Ursacetus.h"
#include "Monster_AdvancedCombatDroid_5.h"
#include "Monster_Onyscidus.h"
#include "HeightMapImage.h"

#define GAME_WORLD GameWorld::GetInst()

class GameWorld
{
	DECLARE_SINGLETON(GameWorld);

private:
	SPtr<Monster_Ursacetus>				m_Mon_Ursacetus[100]{};
	SPtr<Monster_AdvancedCombatDroid_5>	m_Mon_AndCombat5[100]{};
	SPtr<Monster_Onyscidus>				m_Mon_Onyscidus[100]{};

	SPtr<HeightMapImage>				mHeightmapImage;

public:
	GameWorld();
	~GameWorld();

public:

	void Init();

	// ::PostQueuedCompletionStatus()
	void PQCS(OverlappedObject* over);

public:
	SPtr_NetObj GetUrsacetusSPtr(int idx) { return m_Mon_Ursacetus[idx]; }
	SPtr_NetObj GetAdvCombat5(int idx) { return m_Mon_AndCombat5[idx]; }
	SPtr_NetObj GetOnyscidus(int idx) { return m_Mon_Onyscidus[idx]; }

};

