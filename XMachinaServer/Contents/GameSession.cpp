#include "pch.h"
#include "GameSession.h"

GameSession::GameSession() 
{
}

GameSession::~GameSession()
{
}

void GameSession::OnConnected()
{
}

void GameSession::OnDisconnected()
{
}



void GameSession::OnSend(UINT32 len)
{
}

UINT32 GameSession::OnRecv(BYTE* buffer, UINT32 len)
{
	// ��Ŷ �ؼ� 
	std::cout << static_cast<void*>(buffer) << "  : Bytes-" << len << std::endl;


	return len;
}
