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

	ROOM_MGR->EnterInRoom(mPlayer); // WRITE Lock 

	/* [DB] User Info */
	mUserInfo = MEMORY->Make_Shared<DB_UserInfo>();
	mUserInfo->SetOnwerGameSession(std::dynamic_pointer_cast<GameSession>(shared_from_this()));
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
	// ��Ŷ �ؼ� 
	/* ���ļ� ���� ��Ŷ���� ó���Ѵ�. */
	/// +---------------------------------------
	///	[ Packet 1 ][ Packet 2 ]...[ Packet N ]
	///	��			��
	/// buffer     (buffer + ProcessDataSize)
	/// ---------------------------------------+
	// ������� ó���� ������ ũ��
	UINT32 ProcessDataSize = 0;

	// ���� ���� �����͸� ����Ͽ� ��ü ũ�⸦ ���
	UINT32 TotalSize = len;

	while (ProcessDataSize < TotalSize) {
		UINT32 RemainSize = TotalSize - ProcessDataSize;

		// ���� �����Ͱ� PacketHeader�� ũ�⺸�� ������ ���� �� ���ſ��� ó��
		if (RemainSize < sizeof(PacketHeader)) {
			break;
		}

		// ��Ŷ ����� �Ľ� 
		BYTE* startBufferPtr = buffer + ProcessDataSize;
		PacketHeader* packet = reinterpret_cast<PacketHeader*>(startBufferPtr);

		// ���� �����Ͱ� ��Ŷ ��ü ũ�⺸�� ������ ���� �� ���ſ��� ó�� 
		if (RemainSize < packet->PacketSize) {
			break;
		}

		// ��Ŷ ó��
		FBsPacketFactory::ProcessFBsPacket(static_pointer_cast<Session>(shared_from_this()), startBufferPtr, packet->PacketSize);

		// ó���� ������ ũ�⸸ŭ ���� 
		ProcessDataSize += packet->PacketSize;
	}
	// ���� ������ ũ�� ���� 
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
