#include "pch.h"
#include "GameSession.h"

#include "../Framework.h"
#include "Protocol/FBsPacketFactory.h"
#include "../ServerLib/MemoryManager.h"
#include "GameManager.h"
#include "GamePlayer.h"

#include "Script_Player.h"

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
	mPlayer->AddComponent<Transform>(ComponentInfo::Type::Transform);
	mPlayer->AddComponent<Collider>(ComponentInfo::Type::Collider);
	mPlayer->AddScript<Script_Player>(ScriptInfo::Type::Stat);


	GAME_MGR->EnterInRoom(mPlayer); // WRITE Lock 
}

void GameSession::OnDisconnected()
{
	SPtr_SendPktBuf removePkt = FBS_FACTORY->SPkt_RemovePlayer(GetID());
	GAME_MGR->BroadcastRoom(mPlayer->GetRoomID(), removePkt, GetID()); /* SEND REMOVE PKT TO SESSIONS IN ROOM */
	GAME_MGR->ExitInRoom(mPlayer); // WRITE Lock
	mPlayer = nullptr; // Dec Ref 

}

void GameSession::OnSend(UINT32 len)
{
}

UINT32 GameSession::OnRecv(BYTE* buffer, UINT32 len)
{
	// 패킷 해석 
	/* 뭉쳐서 들어온 패킷들을 처리한다. */
	/// +---------------------------------------
	///	[ Packet 1 ][ Packet 2 ]...[ Packet N ]
	///	↑			↑
	/// buffer     (buffer + PRocessDataSize)
	/// ---------------------------------------+

	UINT32 ProcessDataSize = mRemainDataSize;
	while (ProcessDataSize < len) {
		UINT32 RemainSize = len - ProcessDataSize;
		if (RemainSize < sizeof(PacketHeader)) {
			mRemainDataSize = RemainSize;
			break;
		}
		PacketHeader* packet = reinterpret_cast<PacketHeader*>(buffer + ProcessDataSize);
		if (RemainSize < packet->PacketSize) {
			mRemainDataSize = RemainSize;
			break;
		}

		/* 패킷 해석 */
		FBsPacketFactory::ProcessFBsPacket(static_pointer_cast<Session>(shared_from_this()), buffer + ProcessDataSize, packet->PacketSize);
		ProcessDataSize += packet->PacketSize; 
	}

	return len;
}
