#include "pch.h"
#include "GameManager.h"
#include "GameRoom.h"
#include "Framework.h"
#include "ServerLib/NetworkInterface.h"
#include "ServerLib/ServerNetwork.h"

DEFINE_SINGLETON(GameManager);

GameManager::GameManager()
{

}

GameManager::~GameManager()
{
	for (UINT32 i = 0; i < mRooms.size(); ++i) {
		mRooms[i] = nullptr;
	}
	mRooms.clear();
}

void GameManager::Init()
{
	mRoomMaxCnt = 5000 / static_cast<UINT32>(RoomInfo::MaxSessionSize);

	for (UINT32 i = 0; i < mRoomMaxCnt; ++i) {
		SPtr_GameRoom room = std::make_shared<GameRoom>();
		room->SetRoomID(i);
		mRooms.push_back(room);
	}


}


bool GameManager::EnterInRoom(SPtr_GamePlayer player, int roomid)
{
	// ENTER IN ANY POSSIBLE ROOM 
	mSRWLock.LockWrite();

	if (roomid == -1) {
		for (UINT32 i = 0; i < mRoomMaxCnt; ++i) {
			bool checkin = mRooms[i].get()->CheckIn();
			if (checkin) {
				mRooms[i].get()->EnterPlayer(player);

				mSRWLock.UnlockWrite();
				return true;
			}
		}

	}
	// ENTER IN SPECIFIC ROOM 
	else if (roomid >= 0 && roomid < static_cast<int>(mRoomMaxCnt)) {
		bool checkin = mRooms[roomid].get()->CheckIn();
		if (checkin) {
			mRooms[roomid].get()->EnterPlayer(player);
			mSRWLock.UnlockWrite();
			return true;

		}
	}

	mSRWLock.UnlockWrite();
	return false;
}

bool GameManager::ExitInRoom(SPtr_GamePlayer player)
{
	mSRWLock.LockWrite();

	mRooms[player->GetInfo().RoomID].get()->ExitPlayer(player->GetID());
	mSRWLock.UnlockWrite();

	return true;
}

void GameManager::BroadcastRoom(int roomid, SPtr_SendPktBuf& pkt, int exceptsessionid)
{
	mSRWLock.LockRead();

	mRooms[roomid].get()->Broadcast(pkt, exceptsessionid);

	mSRWLock.UnlockRead();
}

std::vector<PlayerInfo> GameManager::GetPlayerInfos_Room(int roomid)
{
	std::vector<PlayerInfo> playerInfos = mRooms[roomid].get()->GetInsertedPlayersInfo(); // READ Lock
	return playerInfos;
}

