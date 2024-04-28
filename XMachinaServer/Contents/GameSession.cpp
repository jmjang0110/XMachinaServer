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
	// ��Ŷ �ؼ� 
	std::cout << this->GetID() << " - START - RECV : " << static_cast<void*>(buffer) << "  : Bytes-" << len << std::endl;
	
	int ProcessDataSize = 0;
	/* ���ļ� ���� ��Ŷ���� ó���Ѵ�. */
	while (ProcessDataSize < len) {
		UINT32 RemainSize = len - ProcessDataSize;
		if (RemainSize < sizeof(PacketHeader))
			break;
		PacketHeader* packet = reinterpret_cast<PacketHeader*>(buffer + ProcessDataSize);
		if (RemainSize < packet->PacketSize) {
			mRemainDataSize = RemainSize;
			break;
		}

		/* ��Ŷ �ؼ� */
		FBsPacketFactory::ProcessFBsPacket(static_pointer_cast<Session>(shared_from_this()), buffer, len);
		//std::cout << "GAMESESSION ON RECV : " << static_cast<void*>(buffer) << " " << packet->PacketSize << std::endl;
		ProcessDataSize += packet->PacketSize; 
	}
	std::cout << this->GetID() << " - RECV : " << static_cast<void*>(buffer) << "  : Bytes-" << ProcessDataSize << std::endl;

	this->Send(FRAMEWORK->GetSendFactory()->SPkt_NewtorkLatency(1234)); // Ŭ���̾�Ʈ�� �޾Ҵ� �ð� �״�θ� ���� ��Ŷ���� ���� �ٽ� Ŭ�󿡰� ������. 


	return len;
}
