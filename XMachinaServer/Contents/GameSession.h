#pragma once

#include "../ServerLib/Session.h"

class GameSession : public Session
{
private:

public:
	GameSession();
	virtual ~GameSession();


	virtual void	OnConnected() override;
	virtual void	OnDisconnected() override;
	virtual void	OnSend(UINT32 len) override;
	virtual UINT32	OnRecv(BYTE* buffer, UINT32 len) override;

};

