#pragma once

#include "../ServerLib/Session.h"
#include "GamePlayer.h"


class GameSession : public Session
{
private:
	UINT32 mRemainDataSize = 0;
	
	SPtr_GamePlayer mPlayer = {};

public:
	GameSession();
	virtual ~GameSession();


	virtual void	OnConnected() override;
	virtual void	OnDisconnected() override;
	virtual void	OnSend(UINT32 len) override;
	virtual UINT32	OnRecv(BYTE* buffer, UINT32 len) override;

public:
	SPtr_GamePlayer GetPlayer() { return mPlayer; };
	PlayerInfo GetPlayerInfo() { return mPlayer->GetInfo(); };

};

