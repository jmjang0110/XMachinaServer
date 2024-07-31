#pragma once

#include "../ServerLib/Session.h"
#include "GamePlayer.h"


class GameSession : public Session
{
private:
	UINT32 mRemainDataSize = 0;
	
	SPtr_GamePlayer mPlayer = {}; // �ش� ������ �����ϴ� GameWorld ������ Player 

public:
	GameSession();
	virtual ~GameSession();


	virtual void	OnConnected() override;
	virtual void	OnDisconnected() override;
	virtual void	OnSend(UINT32 len) override;
	virtual UINT32	OnRecv(BYTE* buffer, UINT32 len) override;

public:
	SPtr_GamePlayer GetPlayer() { return mPlayer; };
	//PlayerSnapShot GetPlayerSnapShot() { return mPlayer->GetSnapShot(); };

};

