#include "pch.h"
#include "GameSession.h"
#include "Protocol/FBsPacketFactory.h"

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
	// 패킷 해석 
	//std::cout << "RECV : " << static_cast<void*>(buffer) << "  : Bytes-" << len << std::endl;
	FBsPacketFactory::ProcessFBsPacket(static_pointer_cast<Session>(shared_from_this()), buffer, len);

	return len;
}
