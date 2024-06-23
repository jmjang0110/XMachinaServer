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

