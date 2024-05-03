#include "pch.h"
#include "GamePlayer.h"


GamePlayer::GamePlayer()
	: GameObject()
{
}

GamePlayer::GamePlayer(UINT32 sessionID, SPtr_GameSession owner)
	: GameObject(sessionID)
{
	mInfo.PlayerID = sessionID;
	mInfo.Owner    = owner;
}

GamePlayer::~GamePlayer()
{
	mInfo.Owner = nullptr;
}
