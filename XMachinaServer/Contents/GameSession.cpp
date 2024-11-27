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

#include "DB_UserInfo.h"


GameSession::GameSession() 
{
}

GameSession::~GameSession()
{ 
	if (mPlayer) {
		mPlayer->DeActivate();
		mPlayer = nullptr;
	}
	mUserInfo = nullptr;
}

void GameSession::OnConnected()
{
	/* Player */
	mPlayer = MEMORY->Make_Shared<GameObject>(this->GetID());
	mPlayer->AddComponent<Transform>(Component::Type::Transform);
	mPlayer->AddComponent<Collider>(Component::Type::Collider);
	auto player_entity = mPlayer->SetScriptEntity<Script_Player>();
	player_entity->SetSessionOwner(std::dynamic_pointer_cast<GameSession>(shared_from_this()));

	mPlayer->Start();

	if (!ROOM_MGR->EnterInRoom(mPlayer)) // WRITE Lock 
	{
		LOG_MGR->Cout("Enter In Room Failed : ", mPlayer->GetID(),"\n");
	}
	LOG_MGR->Cout(ROOM_MGR->session_count.load(), "\n");


	/* [DB] User Info */
	mUserInfo = MEMORY->Make_Shared<DB_UserInfo>();
	mUserInfo->SetOnwerGameSession(std::dynamic_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected()
{
	LOG_MGR->Cout("-- OnDisconnected ", mID, "\n");

	// Room 에 들어가지 못한 Session
	if (mPlayer->GetOwnerRoom() == nullptr) {
		mPlayer = nullptr;
		return;
	}

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
	/// buffer     (buffer + ProcessDataSize)
	/// ---------------------------------------+
	// 현재까지 처리된 데이터 크기
	UINT32 ProcessDataSize = 0;

	// 기존 남은 데이터를 고려하여 전체 크기를 계산
	UINT32 TotalSize = len;

	while (ProcessDataSize < TotalSize) {
		UINT32 RemainSize = TotalSize - ProcessDataSize;

		// 남은 데이터가 PacketHeader의 크기보다 작으면 다음 번 수신에서 처리
		if (RemainSize < sizeof(PacketHeader)) {
			break;
		}

		// 패킷 헤더를 파싱 
		BYTE* startBufferPtr = buffer + ProcessDataSize;
		PacketHeader* packet = reinterpret_cast<PacketHeader*>(startBufferPtr);

		// 남은 데이터가 패킷 전체 크기보다 적으면 다음 번 수신에서 처리 
		if (RemainSize < packet->PacketSize) {
			break;
		}

		// 패킷 처리
		FBsPacketFactory::ProcessFBsPacket(static_pointer_cast<Session>(shared_from_this()), startBufferPtr, packet->PacketSize);

		// 처리된 데이터 크기만큼 증가 
		ProcessDataSize += packet->PacketSize;
	}
	// 남은 데이터 크기 갱신 
	mRemainDataSize = TotalSize - ProcessDataSize;

	return ProcessDataSize;
}

void GameSession::LoadUserInfo(std::string& id, std::string& password)
{
	mUserInfo->LoadFromDataBase(id, password);
}

void GameSession::UpdateUserInfo()
{
	mPlayer->SetName(mUserInfo->Name);
}

SPtr<Script_Player> GameSession::GetPlayerEntity()
{
	return mPlayer->GetScriptEntity<Script_Player>();
}
