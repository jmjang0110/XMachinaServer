#pragma once

#include "ServerLib/Session.h"

class GameObject;
class Script_Player;
class GameSession : public Session
{
private:
	UINT32			 mRemainDataSize = 0;
	SPtr<GameObject> mPlayer         = {}; // �ش� ������ �����ϴ� GameWorld ������ Player 

public:
	GameSession();
	virtual ~GameSession();

	virtual void	OnConnected() override;
	virtual void	OnDisconnected() override;
	virtual void	OnSend(UINT32 len) override;
	virtual UINT32	OnRecv(BYTE* buffer, UINT32 len) override;

public:
	SPtr<GameObject> GetPlayer() { return mPlayer; };
	SPtr<Script_Player> GetPlayerEntity();

};

