#include "pch.h"
#include "GameSession.h"

#include "../Framework.h"
#include "Protocol/FBsPacketFactory.h"
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
	SPtr_SendPktBuf removePkt = FBS_FACTORY->SPkt_RemovePlayer(GetID());
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
	std::cout << this->GetID() << " RECV : " << len << std::endl;
	
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

	return len;
}
