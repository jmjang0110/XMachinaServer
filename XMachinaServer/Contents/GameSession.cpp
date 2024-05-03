#include "pch.h"
#include "GameSession.h"
#include "Protocol/FBsPacketFactory.h"
#include "../Framework.h"
#include "../ServerLib/SendBuffersFactory.h"
#include "../ServerLib/MemoryManager.h"
#include "GameManager.h"
#include "GamePlayer.h"


GameSession::GameSession() 
{
}

GameSession::~GameSession()
{
	if (mPlayer)
		mPlayer->DecRef_OwnerGameSession();
}

void GameSession::OnConnected()
{
	mPlayer = MEMORY->Make_Shared<GamePlayer>(this->GetID(), std::static_pointer_cast<GameSession>(shared_from_this()));
	GAME_MGR->EnterInRoom(mPlayer); // WRITE Lock 
}

void GameSession::OnDisconnected()
{
	SPtr_SendPktBuf removePkt = SEND_FACTORY->SPkt_RemovePlayer(GetID());
	GAME_MGR->BroadcastRoom(GetPlayerInfo().RoomID, removePkt, GetID()); /* SEND REMOVE PKT TO SESSIONS IN ROOM */
	GAME_MGR->ExitInRoom(mPlayer); // WRITE Lock
	mPlayer = nullptr; // Dec Ref 

}

void GameSession::OnSend(UINT32 len)
{
}

UINT32 GameSession::OnRecv(BYTE* buffer, UINT32 len)
{
	// ��Ŷ �ؼ� 
	std::cout << this->GetID() << "RECV : " << static_cast<void*>(buffer) << "  : Bytes-" << len << std::endl;
	
	UINT32 ProcessDataSize = 0;
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

	// TEST 
	this->Send(FRAMEWORK->GetSendFactory()->SPkt_NewtorkLatency(1234)); // Ŭ���̾�Ʈ�� �޾Ҵ� �ð� �״�θ� ���� ��Ŷ���� ���� �ٽ� Ŭ�󿡰� ������. 


	return len;
}
