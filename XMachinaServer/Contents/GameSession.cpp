#include "pch.h"
#include "GameSession.h"
#include "Protocol/FBsPacketFactory.h"
#include "../Framework.h"
#include "../ServerLib/SendBuffersFactory.h"

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
	std::cout << this->GetID() << " - START - RECV : " << static_cast<void*>(buffer) << "  : Bytes-" << len << std::endl;
	
	int ProcessDataSize = 0;
	/* 뭉쳐서 들어온 패킷들을 처리한다. */
	while (ProcessDataSize < len) {
		UINT32 RemainSize = len - ProcessDataSize;
		if (RemainSize < sizeof(PacketHeader))
			break;
		PacketHeader* packet = reinterpret_cast<PacketHeader*>(buffer + ProcessDataSize);
		if (RemainSize < packet->PacketSize) {
			mRemainDataSize = RemainSize;
			break;
		}

		/* 패킷 해석 */
		FBsPacketFactory::ProcessFBsPacket(static_pointer_cast<Session>(shared_from_this()), buffer, len);
		//std::cout << "GAMESESSION ON RECV : " << static_cast<void*>(buffer) << " " << packet->PacketSize << std::endl;
		ProcessDataSize += packet->PacketSize; 
	}
	std::cout << this->GetID() << " - RECV : " << static_cast<void*>(buffer) << "  : Bytes-" << ProcessDataSize << std::endl;

	this->Send(FRAMEWORK->GetSendFactory()->SPkt_NewtorkLatency(1234)); // 클라이언트에 받았던 시간 그대로를 서버 패킷으로 만들어서 다시 클라에게 보낸다. 


	return len;
}
