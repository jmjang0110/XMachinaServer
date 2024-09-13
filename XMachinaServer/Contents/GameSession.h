#pragma once

#include "ServerLib/Session.h"

class GameObject;
class Script_Player;
class DB_UserInfo;
class GameSession : public Session
{
private:
	UINT32				mRemainDataSize = 0;
	SPtr<GameObject>	mPlayer         = {}; // �ش� ������ �����ϴ� GameWorld ������ Player 
	SPtr<DB_UserInfo>	mUserInfo       = {};

public:
	GameSession();
	virtual ~GameSession();

	virtual void	OnConnected() override;
	virtual void	OnDisconnected() override;
	virtual void	OnSend(UINT32 len) override;
	virtual UINT32	OnRecv(BYTE* buffer, UINT32 len) override;

public:
	void LoadUserInfo(std::string& id, std::string& password);


public:
	SPtr<GameObject> GetPlayer() { return mPlayer; };
	SPtr<Script_Player> GetPlayerEntity();

};

