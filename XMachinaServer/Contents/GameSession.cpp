#include "pch.h"
#include "GameSession.h"

#include "Framework.h"
#include "Protocol/FBsPacketFactory.h"
#include "ServerLib/MemoryManager.h"
#include "RoomManager.h"
#include "GameRoom.h"
#include "GameObject.h"

#include "Script.h"
#include "Script_Player.h"

GameSession::GameSession() 
{
}

GameSession::~GameSession()
{ 
	if (mPlayer) {
		mPlayer->DeActivate();
		mPlayer = nullptr;
	}
}

void GameSession::OnConnected()
{
	mPlayer = MEMORY->Make_Shared<GameObject>(this->GetID());
	mPlayer->AddComponent<Transform>(Component::Type::Transform);
	mPlayer->AddComponent<Collider>(Component::Type::Collider);
	auto player_entity = mPlayer->SetScriptEntity<Script_Player>();
	player_entity->SetSessionOwner(std::dynamic_pointer_cast<GameSession>(shared_from_this()));

	mPlayer->Start();

	ROOM_MGR->EnterInRoom(mPlayer); // WRITE Lock 
}

void GameSession::OnDisconnected()
{
	SPtr_SendPktBuf removePkt = FBS_FACTORY->SPkt_RemovePlayer(GetID());
	ROOM_MGR->BroadcastRoom(mPlayer->GetOwnerRoom()->GetID(), removePkt, GetID()); /* SEND REMOVE PKT TO SESSIONS IN ROOM */
	ROOM_MGR->ExitInRoom(mPlayer); // WRITE Lock
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

SPtr<Script_Player> GameSession::GetPlayerEntity()
{
	return mPlayer->GetScriptEntity<Script_Player>();
}
