#include "pch.h"
#include "RoomManager.h"
#include "GameRoom.h"
#include "GameObject.h"
#include "Framework.h"
#include "ServerLib/NetworkInterface.h"
#include "ServerLib/ServerNetwork.h"

DEFINE_SINGLETON(RoomManager);

RoomManager::RoomManager()
{

}

RoomManager::~RoomManager()
{
	for (UINT32 i = 0; i < mRooms.size(); ++i) {
		mRooms[i] = nullptr;
	}
	mRooms.clear();
}

void RoomManager::Init()
{
	mRoomMaxCnt = MAX_SESSION_NUM / static_cast<UINT32>(RoomInfo::MaxSessionSize);


	auto start = std::chrono::high_resolution_clock::now();

	for (UINT32 i = 0; i < mRoomMaxCnt; ++i) {
		SPtr<GameRoom> room = std::make_shared<GameRoom>();
		room->Init(i);
		mRooms.push_back(room);
	}


	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "RoomManager - Room Init " << duration.count() << " seconds" << std::endl;

}


bool RoomManager::EnterInRoom(SPtr<GameObject> player, int roomid)
{
	// ENTER IN ANY POSSIBLE ROOM 
	if (roomid == -1) {
		for (UINT32 i = 0; i < mRooms.size(); ++i) {
			bool checkin = mRooms[i].get()->IsPossibleToEnter();
			if (checkin) {
				mRooms[i].get()->EnterPlayer(player);
				session_count++;
				return true;
			}
		}

	}
	// ENTER IN SPECIFIC ROOM 
	else if (roomid >= 0 && roomid < static_cast<int>(mRoomMaxCnt)) {
		bool checkin = mRooms[roomid].get()->IsPossibleToEnter();
		if (checkin) {
			mRooms[roomid].get()->EnterPlayer(player);
			session_count++;
			return true;

		}
	}
	return false;
}

bool RoomManager::ExitInRoom(SPtr<GameObject> player)
{
	mRooms[player->GetOwnerRoom()->GetID()].get()->ExitPlayer(player->GetID());
	return true;
}

void RoomManager::BroadcastRoom(int roomid, SPtr_SendPktBuf& pkt, int exceptsessionid)
{
	mRooms[roomid].get()->Broadcast(pkt, exceptsessionid);
}

void RoomManager::BroadcastAllRoom(SPtr_SendPktBuf& pkt)
{
	for (int i = 0; i < mRooms.size(); ++i) {
		mRooms[i].get()->Broadcast(pkt);
	}
}

void RoomManager::Send(SPtr_SendPktBuf& pkt, int roomID, int sessionID)
{
	mRooms[roomID]->SendPacket(static_cast<UINT32>(sessionID), pkt);
}

std::vector<SPtr<GameObject>> RoomManager::GetAllPlayersInRoom(int roomID)
{
	std::vector<SPtr<GameObject>> players = mRooms[roomID]->GetallPlayers();
	return players;
}

