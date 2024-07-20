#include "pch.h"
#include "PlayerController.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "ServerLib/ThreadManager.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}


bool PlayerController::EnterPlayer(SPtr_GamePlayer player)
{
	mCurrPlayerCnt.fetch_add(1);

	{
		mSRWLock.LockWrite();

		mGamePlayers[player->GetID()] = player;
		player->setRoomID(mRoomID);
		player->SetOwnerPlayerController(this);

		mSRWLock.UnlockWrite();
	}


	LOG_MGR->SetColor(TextColor::BrightCyan);
	LOG_MGR->Cout(mRoomID, " - ROOM - ", player->GetID(), " ENTER SUCCESS\n");
	LOG_MGR->SetColor(TextColor::Default);


	return true;
}

void PlayerController::Init(int roomID, SPtr_GameRoom owner)
{
	mRoomID    = roomID;
	mOwnerRoom = owner;

}

bool PlayerController::ExitPlayer(UINT32 sessionID)
{
	mCurrPlayerCnt.fetch_sub(1);

	{
		mSRWLock.LockWrite();
		mGamePlayers[sessionID]->Exit();
		mGamePlayers.unsafe_erase(sessionID);
		mSRWLock.UnlockWrite();
	}

	LOG_MGR->SetColor(TextColor::BrightMagenta);
	LOG_MGR->Cout(mRoomID, " - ROOM ", sessionID, " EXIT SUCCESS\n");
	LOG_MGR->SetColor(TextColor::Default);


	return true;
}

SPtr_GamePlayer PlayerController::FindPlayer(UINT32 sessionID)
{
	//mSRWLock.LockRead();

	size_t size = mGamePlayers.size();
	auto obj = mGamePlayers.find(sessionID);
	if (obj == mGamePlayers.end())
	{
		//mSRWLock.UnlockRead();
		return nullptr;
	}

	//mSRWLock.UnlockRead();
	return obj->second;
}

void PlayerController::Broadcast(SPtr_SendPktBuf spkt, UINT32 exceptSessionID)
{
	mSRWLock.LockRead();

	for (auto& player : mGamePlayers) {
		if (player.first == exceptSessionID) continue;
		SPtr_GameSession session = player.second->GetSessionOwner();
		session->Send(spkt);

	}

	mSRWLock.UnlockRead();
}

void PlayerController::SendPacket(UINT32 sessionID, SPtr_SendPktBuf sendPkt)
{
	mSRWLock.LockRead();

	const auto& iter = mGamePlayers.find(sessionID);
	if (iter != mGamePlayers.end()) {
		SPtr_GameSession session = iter->second->GetSessionOwner();
		if (sendPkt != nullptr)
			session->Send(sendPkt);
	}

	mSRWLock.UnlockRead();
}

std::vector<PlayerSnapShot> PlayerController::GetInsertedPlayersInfo()
{
	mSRWLock.LockRead();

	std::vector<PlayerSnapShot> PlayerSnapShots = {};
	for (auto& player : mGamePlayers) {
		PlayerSnapShots.push_back(player.second->GetSnapShot());
	}

	mSRWLock.UnlockRead();

	return PlayerSnapShots;
}

std::vector<SPtr<GamePlayer>> PlayerController::GetPlayersInViewRange(Vec3 player_pos, float viewrange_radius)
{
	std::vector<SPtr<GamePlayer>> playersInView;
	for (auto& player : mGamePlayers) {
		PlayerSnapShot snapShot = player.second->GetSnapShot();


		// x, z 좌표 간의 거리 계산
		float distance = static_cast<float>(std::sqrt(std::pow(snapShot.Position.x - player_pos.x, 2) + std::pow(snapShot.Position.z - player_pos.z, 2)));

		// 거리 비교
		if (distance <= viewrange_radius) {
			playersInView.push_back(player.second);
		}
	}

	return playersInView;
}

