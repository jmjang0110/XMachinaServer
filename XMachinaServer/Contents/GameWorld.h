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

#define GAME_WORLD GameWorld::GetInst()

class GameWorld
{
	DECLARE_SINGLETON(GameWorld);

private:
	std::shared_ptr<Monster_Ursacetus> m_Mon_Ursacetus[100]{};

public:
	GameWorld();
	~GameWorld();

public:

	void Init();

	// ::PostQueuedCompletionStatus()
	void PQCS(OverlappedObject* over);

public:
	SPtr_NetObj GetUrsacetusSPtr(int idx) { return m_Mon_Ursacetus[idx]; }

};

