#pragma once


/// +-------------------------------
///		     GameWorld
/// ________________________________
///
///	�������� ���� Game ���¸� ���� �����Ѵ�.
/// Game World�� ���¸� ������Ʈ�ϰ� �����ϴ� �����̴�.
/// 
/// GameWorld�� ������Ʈ �Ǹ� ������Ʈ ������ 
/// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ���� ������ GameWorld�� ����ȭ�Ѵ�.

/// -------------------------------+

#include "Monster_Ursacetus.h"
#include "Monster_AdvancedCombatDroid_5.h"
#include "Monster_Onyscidus.h"
#include "HeightMapImage.h"

#define GAME_WORLD GameWorld::GetInst()

class GameWorld
{
	DECLARE_SINGLETON(GameWorld);

//private:
//	SPtr<Monster_Ursacetus>				m_Mon_Ursacetus[100]{};
//	SPtr<Monster_AdvancedCombatDroid_5>	m_Mon_AndCombat5[100]{};
//	SPtr<Monster_Onyscidus>				m_Mon_Onyscidus[100]{};
//
//	SPtr<HeightMapImage>				mHeightmapImage;
//
//public:
//	GameWorld();
//	~GameWorld();
//
//public:
//
//	void Init();
//
//	// ::PostQueuedCompletionStatus()
//	void PQCS(OverlappedObject* over);
//
//public:
//	SPtr_NetObj GetUrsacetusSPtr(int idx) { return m_Mon_Ursacetus[idx]; }
//	SPtr_NetObj GetAdvCombat5(int idx) { return m_Mon_AndCombat5[idx]; }
//	SPtr_NetObj GetOnyscidus(int idx) { return m_Mon_Onyscidus[idx]; }

};

