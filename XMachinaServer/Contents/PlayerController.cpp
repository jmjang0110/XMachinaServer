#include "pch.h"
#include "PlayerController.h"
#include "GameRoom.h"
#include "GameObject.h"
#include "GameSession.h"

/* Player Script */
#include "Script_Player.h"

SPtr<GameObject> PlayerController::GetPlayer(UINT32 ID)
{

	mSRWLock.LockRead();

	auto it = mPlayers.find(ID);
	if (it != mPlayers.end()) {
		mSRWLock.UnlockRead();
		return it->second;
	}

	mSRWLock.UnlockRead();
	return nullptr;
}

std::vector<SPtr<GameObject>> PlayerController::GetAllPlayers()
{
	std::vector<SPtr<GameObject>> allPlayers;
	mSRWLock.LockRead();

	for (auto& iter : mPlayers) {
		allPlayers.push_back(iter.second);
	}

	mSRWLock.UnlockRead();
	return allPlayers;
}

std::vector<std::pair<UINT32, Vec3>> PlayerController::GetPlayersPosition()
{
	std::vector<std::pair<UINT32, Vec3>> result;
	for (auto& iter : mPlayers) { 

		int  id                       = iter.second->GetID();
		auto playerTransformSnapShot  = iter.second->GetTransform()->GetSnapShot();
		Vec3 pos                      = playerTransformSnapShot.GetPosition();
		result.push_back(std::make_pair(id, pos));

	}
	return result;
}

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}


bool PlayerController::EnterPlayer(SPtr<GameObject> player)
{
	mCurrPlayerCnt.fetch_add(1);

	{
		mSRWLock.LockWrite();

		player->SetOwnerRoom(mOwnerRoom);
		auto player_entity = player->GetScriptEntity<Script_Player>();
		if (player_entity) {
			mPlayers[player->GetID()] = player;
			player->Start();
		}
		else
			assert(0);


		mSRWLock.UnlockWrite();
	}


	LOG_MGR->SetColor(TextColor::BrightCyan);
	LOG_MGR->Cout(mRoomID, " - ROOM - ", player->GetID(), " ENTER SUCCESS\n");
	LOG_MGR->SetColor(TextColor::Default);


	return true;
}

void PlayerController::Init(int roomID, SPtr<GameRoom> owner)
{
	mRoomID    = roomID;
	mOwnerRoom = owner;

}

bool PlayerController::ExitPlayer(UINT32 sessionID)
{
	mCurrPlayerCnt.fetch_sub(1);

	{
		mSRWLock.LockWrite();
		SPtr<Script_Player> player_entity = mPlayers[sessionID]->GetScriptEntity<Script_Player>();
		player_entity->SetState(PlayerState::Exit);
		mPlayers.unsafe_erase(sessionID);
		mSRWLock.UnlockWrite();
	}

	LOG_MGR->SetColor(TextColor::BrightMagenta);
	LOG_MGR->Cout(mRoomID, " - ROOM ", sessionID, " EXIT SUCCESS\n");
	LOG_MGR->SetColor(TextColor::Default);


	return true;
}

SPtr<GameObject> PlayerController::FindPlayer(UINT32 sessionID)
{
	//mSRWLock.LockRead();

	size_t size = mPlayers.size();
	auto obj = mPlayers.find(sessionID);
	if (obj == mPlayers.end())
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

	for (auto& player : mPlayers) {
		if (player.first == exceptSessionID) continue;
		auto player_entity = player.second->GetScriptEntity<Script_Player>();
		SPtr<GameSession> session = player_entity->GetSessionOwner();
		if(session)
			session->Send(spkt);
	}

	mSRWLock.UnlockRead();
}

void PlayerController::SendPacket(UINT32 sessionID, SPtr_SendPktBuf sendPkt)
{
	mSRWLock.LockRead();

	const auto& iter = mPlayers.find(sessionID);
	if (iter != mPlayers.end()) {

		auto			  player_entity = iter->second->GetScriptEntity<Script_Player>();
		SPtr<GameSession> session       = player_entity->GetSessionOwner();
		if (sendPkt != nullptr)
			session->Send(sendPkt);
	}

	mSRWLock.UnlockRead();
}



std::vector<SPtr<GameObject>> PlayerController::GetPlayersInViewRange(Vec3 player_pos, float viewrange_radius)
{
	std::vector<SPtr<GameObject>> playersInView;
	for (auto& player : mPlayers) {

		/* Player Transform Snap Shot */
		auto PlayerTransformSnapShot = player.second->GetTransform()->GetSnapShot();
		Vec3 playerPos               = PlayerTransformSnapShot.GetPosition();

		// x, z 좌표 간의 거리 계산
		float distance = static_cast<float>(std::sqrt(std::pow(playerPos.x - player_pos.x, 2) + std::pow(playerPos.z - player_pos.z, 2)));

		// 거리 비교
		if (distance <= viewrange_radius) {
			playersInView.push_back(player.second);
		}
	}

	return playersInView;
}

