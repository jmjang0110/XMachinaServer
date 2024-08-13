#include "pch.h"
#include "FBsPacketFactory.h"
#include "ServerLib/PacketHeader.h"

#include "../Framework.h"
#include "../ServerLib/SendBuffersFactory.h"
#include "../Contents/GameSession.h"
#include "../ServerLib/SocketData.h"
#include "Contents/GamePlayer.h"
#include "Contents/GameMonster.h"
#include "Contents/GameManager.h"
#include "Contents/GameSkill.h"
#include "Contents/NPCController.h"
#include "Contents/PlayerController.h"
#include "Contents/GameRoom.h"	
#include "Contents/GameItem.h"
#include "Contents/Collider.h"


#include "Contents/Script_EnemyController.h"
#include "Contents/Script_Player.h"

DEFINE_SINGLETON(FBsPacketFactory);

/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ PROCESS CLIENT PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★
bool FBsPacketFactory::ProcessFBsPacket(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen)
{
	/* ▶ Packet Struct ------------------------------------------------- */
	/* [PacketHeader(ProtocolID, PktSize)][DATA-(FlatBuffers Serialized)] */
	/* ------------------------------------------------------------------ */

	PacketHeader* Head = reinterpret_cast<PacketHeader*>(packetBuf);
	const void* DataPtr = packetBuf + sizeof(PacketHeader);

	switch (Head->ProtocolID)
	{
	case FBsProtocolID::CPkt_LogIn:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_LogIn ]\n");
		const FBProtocol::CPkt_LogIn* packet = flatbuffers::GetRoot<FBProtocol::CPkt_LogIn>(DataPtr);
		if (!packet) return false;
		Process_CPkt_LogIn(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_EnterGame:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_EnterGame ]\n");
		const FBProtocol::CPkt_EnterGame* packet = flatbuffers::GetRoot<FBProtocol::CPkt_EnterGame>(DataPtr);
		if (!packet) return false;
		Process_CPkt_EnterGame(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Chat:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_Chat ]\n");
		const FBProtocol::CPkt_Chat* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Chat>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Chat(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_NetworkLatency:
	{
		// LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_NetworkLatency ]\n");
		const FBProtocol::CPkt_NetworkLatency* packet = flatbuffers::GetRoot<FBProtocol::CPkt_NetworkLatency>(DataPtr);
		if (!packet) return false;
		Process_CPkt_NetworkLatency(session, *packet);
		break;
	}

	/// +--------------------------------
	///		 PROCESS CPKT PLAYER
	/// --------------------------------+
	case FBsProtocolID::CPkt_NewPlayer:
	{
		const FBProtocol::CPkt_NewPlayer* packet = flatbuffers::GetRoot<FBProtocol::CPkt_NewPlayer>(DataPtr);
		if (!packet) return false;
		Process_CPkt_NewPlayer(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_RemovePlayer:
	{
		const FBProtocol::CPkt_RemovePlayer* packet = flatbuffers::GetRoot<FBProtocol::CPkt_RemovePlayer>(DataPtr);
		if (!packet) return false;
		Process_CPkt_RemovePlayer(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Player_Transform:
	{
		const FBProtocol::CPkt_Player_Transform* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_Transform>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_Transform(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Player_Animation:
	{
		const FBProtocol::CPkt_Player_Animation* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_Animation>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_Animation(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Player_Weapon:
	{
		const FBProtocol::CPkt_Player_Weapon* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_Weapon>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_Weapon(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Player_AimRotation:
	{
		const FBProtocol::CPkt_Player_AimRotation* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_AimRotation>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_AimRotation(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_PlayerOnSkill:
	{
		const FBProtocol::CPkt_PlayerOnSkill* packet = flatbuffers::GetRoot<FBProtocol::CPkt_PlayerOnSkill>(DataPtr);
		if (!packet) return false;
		Process_CPkt_PlayerOnSkill(session, *packet);
		break;
	}
	break;
	case FBsProtocolID::CPkt_Player_State:
	{
		const FBProtocol::CPkt_Player_State* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_State>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_State(session, *packet);
	}
	break;

	/// +--------------------------------
	///		 PROCESS CPKT MONSTER
	/// --------------------------------+
	case FBsProtocolID::CPkt_NewMonster:
	{
		const FBProtocol::CPkt_NewMonster* packet = flatbuffers::GetRoot<FBProtocol::CPkt_NewMonster>(DataPtr);
		if (!packet) return false;
		Process_CPkt_NewMonster(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_RemoveMonster:
	{
		const FBProtocol::CPkt_RemoveMonster* packet = flatbuffers::GetRoot<FBProtocol::CPkt_RemoveMonster>(DataPtr);
		if (!packet) return false;
		Process_CPkt_RemoveMonster(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Monster_Transform:
	{
		const FBProtocol::CPkt_Monster_Transform* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Monster_Transform>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Monster_Transform(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Monster_HP:
	{
		const FBProtocol::CPkt_Monster_HP* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Monster_HP>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Monster_HP(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Monster_State:
	{
		const FBProtocol::CPkt_Monster_State* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Monster_State>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Monster_State(session, *packet);
		break;
	}


	/// +--------------------------------
	///		 PROCESS CPKT BULLET
	/// --------------------------------+
	case FBsProtocolID::CPkt_Bullet_OnShoot:
	{
		const FBProtocol::CPkt_Bullet_OnShoot* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Bullet_OnShoot>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Bullet_OnShoot(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Bullet_OnHitEnemy:
	{
		const FBProtocol::CPkt_Bullet_OnHitEnemy* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Bullet_OnHitEnemy>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Bullet_OnHitEnemy(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Bullet_OnCollision:
	{
		const FBProtocol::CPkt_Bullet_OnCollision* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Bullet_OnCollision>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Bullet_OnCollision(session, *packet);
		break;
	}
	/// +--------------------------------
	///		 PROCESS CPKT ITEM
	/// --------------------------------+
	case FBsProtocolID::CPkt_Item_Interact:
	{
		const FBProtocol::CPkt_Item_Interact* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Item_Interact>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Item_Interact(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Item_ThrowAway:
	{
		const FBProtocol::CPkt_Item_ThrowAway* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Item_ThrowAway>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Item_ThrowAway(session, *packet);
		break;
	}
	default:
	{
		return Process_CPkt_Invalid(session, packetBuf, Datalen);
	}
	}

	return true;
}

bool FBsPacketFactory::Process_CPkt_Invalid(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen)
{
	return true;
}


/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ PROCESS [LogIn, Chat, NetworkLatency, Entergame] Client PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★
bool FBsPacketFactory::Process_CPkt_LogIn(SPtr_Session session, const FBProtocol::CPkt_LogIn& pkt)
{

	///	table CPkt_LogIn
	///	{
	///
	///	}
	/// 

#ifdef CONNECT_WITH_TEST_CLIENT
	return true;
#endif

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	gameSession->GetPlayer()->GetTransform()->SetPosition(Vec3(65, 0, 240));

	LOG_MGR->SetColor(TextColor::BrightBlue);
	LOG_MGR->Cout("LOG IN SESSION ID : ", gameSession->GetID());
	LOG_MGR->WCout(L"-- LOG-IN-IP : IPv4-", gameSession->GetSocketData().GetIpAddress().c_str(), '\n');
	LOG_MGR->SetColor(TextColor::Default);

	gameSession->GetPlayer()->Update();

	/// +--------------------------------
	/// SEND LOG IN PKT TO ME ( SESSION )
	/// --------------------------------+
	/// +---------------------------------------------------------------------------------------------------------------
	bool LogInSuccess  = true;						   // IS MY GAME PLAYER SUCCESS ?
	/// ---------------------------------------------------------------------------------------------------------------+

	auto SendPkt_LogIn     = FBS_FACTORY->SPkt_LogIn(LogInSuccess);
	session->Send(SendPkt_LogIn);
	
	return true;
}

bool FBsPacketFactory::Process_CPkt_EnterGame(SPtr_Session session, const FBProtocol::CPkt_EnterGame& pkt)
{
    /// ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///table CPkt_EnterGame
	///{
	///	player_id: uint;	// 8 bytes
	///}
	/// ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	
	int								ID             = pkt.player_id();
	SPtr<GamePlayer>				MyPlayer       = gameSession->GetPlayer();
	std::vector<SPtr<GamePlayer>>	RemotePlayers  = GAME_MGR->GetAllPlayersInRoom(MyPlayer->GetRoomID());

	auto SendSPkt_EnterGame = FBS_FACTORY->SPkt_EnterGame(MyPlayer, RemotePlayers);
	session->Send(SendSPkt_EnterGame);

	/// +---------------------------------------------------------------------------------------
	/// SEND NEW PLAYER PKT TO SESSIONS IN ROOM ( SESSION->GET ROOM ID ) - EXCEPT ME ( SESSION )
	/// ---------------------------------------------------------------------------------------+
	auto SendPkt_NewPlayer = FBS_FACTORY->SPkt_NewPlayer(MyPlayer);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), SendPkt_NewPlayer, gameSession->GetID());
	return true;
}

bool FBsPacketFactory::Process_CPkt_Chat(SPtr_Session session, const FBProtocol::CPkt_Chat& pkt)
{
	///> ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///> table CPkt_Chat
	///> {
	///>	message: string;	// 가변 크기
	///> }
	///> ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	std::string message = pkt.message()->c_str();
	auto spkt = FBS_FACTORY->SPkt_Chat(session->GetID(), message);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, session->GetID());

	return true;
}

bool FBsPacketFactory::Process_CPkt_NetworkLatency(SPtr_Session session, const FBProtocol::CPkt_NetworkLatency& pkt)
{
	///>  ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///> table CPkt_NetworkLatency
	///> {
	///> 	 timestamp: long;	// 8 bytes
	///> }
	///>  ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);


	// 패킷으로부터 long long으로 시간을 받음
	long long timestamp = pkt.timestamp();

	/* 클라이언트의 패킷을 그대로 다시 보낸다. */
	auto spkt = FBS_FACTORY->SPkt_NetworkLatency(timestamp);

	session->Send(spkt);
	GAME_MGR->Send(spkt, gameSession->GetPlayer()->GetRoomID(), session->GetID());

	return true;
}


/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ PROCESS [ PLAYER ] Client PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

bool FBsPacketFactory::Process_CPkt_NewPlayer(SPtr_Session session, const FBProtocol::CPkt_NewPlayer& pkt)
{   
	/// > ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// > table CPkt_NewPlayer
	/// > {
	/// > 
	/// > }
	/// > ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	return true;
}

bool FBsPacketFactory::Process_CPkt_RemovePlayer(SPtr_Session session, const FBProtocol::CPkt_RemovePlayer& pkt)
{   /// > ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// > table CPkt_RemovePlayer
	/// > {
	/// > 
	/// > }
	/// > ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	return true;
}

bool FBsPacketFactory::Process_CPkt_PlayerOnSkill(SPtr_Session session, const FBProtocol::CPkt_PlayerOnSkill& pkt)
{   
	///> ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///> table CPkt_PlayerOnSkill
	///> {
	///> 	skill_type: PLAYER_SKILL_TYPE;
	///> 
	///> }s
	///> ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	SPtr_GameSession gameSession                          = std::static_pointer_cast<GameSession>(session);

	FBProtocol::PLAYER_SKILL_TYPE	type				  =  pkt.skill_type();
	auto							playerScript          = gameSession->GetPlayer()->GetScript<Script_Player>(ScriptInfo::Type::Stat);
	float							PheroAmount           = playerScript->GetPhero();
	int								mindcontrol_monster_id = pkt.mindcontrol_monster_id();

	/// +---------------------------------------------------------------------------------------
	/// SEND NEW PLAYER PKT TO SESSIONS IN ROOM ( SESSION->GET ROOM ID ) - EXCEPT ME ( SESSION )
	/// ---------------------------------------------------------------------------------------+


	sptr<GameMonster> mindControlMonster = nullptr;
	if (type == FBProtocol::PLAYER_SKILL_TYPE_MIND_CONTROL) {
		auto npcController = gameSession->GetPlayer()->GetOwnerPlayerController()->GetOwnerRoom()->GetNPCController();

		mindControlMonster = npcController->GetMonster(mindcontrol_monster_id);
		if (mindControlMonster != nullptr) {
			gameSession->GetPlayer()->OnSkill(type, mindControlMonster);
		}
	}
	else {
		gameSession->GetPlayer()->OnSkill(type, nullptr);
	}



	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_Transform(SPtr_Session session, const FBProtocol::CPkt_Player_Transform& pkt)
{    
	/// > ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// > table CPkt_Player_Transform
	/// > {
	/// > 	move_state			: PLAYER_MOTION_STATE_TYPE; // 1 byte
	/// > 
	/// > 	latency				: long;						// 8 bytes
	/// > 	velocity			: float;					// 4 bytes
	/// > 	movedir				: Vector3;					// 12 bytes (3 * 4 bytes)
	/// > 	trans				: Transform;				// 24 bytes (Vector3 * 2)
	/// > 
	/// > 	spine_look			: Vector3;					// 12 bytes (3 * 4 bytes)
	/// > 	animparam_h			: float;					// 4 bytes
	/// > 	animparam_v			: float;					// 4 bytes
	/// > }
	/// > ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	UINT32 id = session->GetID();

	int32_t					move_state  = pkt.move_state();
									    
	long long				latency     = pkt.latency();			/* 해당 클라이언트의 평균 Latency (ms)  */
	float					velocity    = pkt.velocity();
	Vec3					movedir     = GetVector3(pkt.movedir());
	Vec3					pos         = GetVector3(pkt.trans()->position());
	Vec3					rot         = GetVector3(pkt.trans()->rotation());
	
	Vec3					spine_look  = GetVector3(pkt.spine_look());
	float					animparam_h = pkt.animparam_h();
	float					animparam_v = pkt.animparam_v();

	/* Boradcast Player's Transform Update */
	SPtr_SendPktBuf SendPkt = FBS_FACTORY->SPkt_Player_Transform(id, move_state, latency, velocity, movedir, pos, rot, spine_look, animparam_h, animparam_v);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), SendPkt, id);


	/* Player Info Update */
	gameSession->GetPlayer()->GetTransform()->SetPosition(pos);
	gameSession->GetPlayer()->GetTransform()->SetLocalRotation(Quaternion::ToQuaternion(rot));
	gameSession->GetPlayer()->Update(); // Player 업데이트 ! 

	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_Animation(SPtr_Session session, const FBProtocol::CPkt_Player_Animation& pkt)
{   
	/// ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// > table CPkt_Player_Animation
	/// > {
	/// > 	animation_upper_index	: int;		// 4 bytes
	/// > 	animation_lower_index	: int;		// 4 bytes
	/// > 	animation_param_h		: float;	// 4 bytes
	/// > 	animation_param_v		: float;	// 4 bytes
	/// > }
	/// ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	int		ObjectID                = session->GetID();

	int32_t animation_upper_idx     = pkt.animation_upper_index();
	int32_t animation_lower_idx     = pkt.animation_lower_index();
	float	animation_param_h       = pkt.animation_param_h();
	float	animation_param_v       = pkt.animation_param_v();


	/* 클라이언트의 패킷을 그대로 다시 보낸다. */
	auto spkt = FBS_FACTORY->SPkt_Player_Animation(ObjectID, animation_upper_idx, animation_lower_idx, animation_param_h, animation_param_v);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, ObjectID);

	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_Weapon(SPtr_Session session, const FBProtocol::CPkt_Player_Weapon& pkt)
{   
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///> table CPkt_Player_Weapon
	///> {
	///> 	weapon_type: WEAPON_TYPE;	// 1 byte
	///> }
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	FBProtocol::ITEM_TYPE weaponType = pkt.weapon_type();
	gameSession->GetPlayer()->S_SetEquipWeapon(weaponType);

	LOG_MGR->Cout(gameSession->GetID(), " - WEAPON TYPE : ", static_cast<int>(weaponType), "\n");


	auto spkt = FBS_FACTORY->SPkt_Player_Weapon(session->GetID(), weaponType);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, session->GetID());

	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_AimRotation(SPtr_Session session, const FBProtocol::CPkt_Player_AimRotation& pkt)
{
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///> table CPkt_Player_AimRotation
	///> {
	///> 	aim_rotation			: float; // Y rotation Euler
	///> }
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	float aim_rotation = pkt.aim_rotation();
	float spine_angle  = pkt.spine_angle();
	auto spkt          = FBS_FACTORY->SPkt_Player_AimRotation(session->GetID(), aim_rotation, spine_angle);

	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, gameSession->GetID());

	//LOG_MGR->Cout(session->GetID(), " : AIM : ", aim_rotation, '\n');
	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_State(SPtr_Session session, const FBProtocol::CPkt_Player_State& pkt)
{
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	
	SPtr<GamePlayer>			  gamePlayer   = gameSession->GetPlayer();
	float						  hp           = gamePlayer->S_GetHp();
	float						  phero        = gamePlayer->S_GetPhero();
	FBProtocol::PLAYER_STATE_TYPE state        = pkt.state_type();

	auto spkt = FBS_FACTORY->SPKt_Player_State(session->GetID(), hp, phero, state);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, session->GetID());

	return true;
}


/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ PROCESS [ MONSTER ] Client PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

bool FBsPacketFactory::Process_CPkt_NewMonster(SPtr_Session session, const FBProtocol::CPkt_NewMonster& pkt)
{   
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// >table CPkt_NewMonster
	/// >{
	/// >
	/// >}
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	return true;
}

bool FBsPacketFactory::Process_CPkt_DeadMonster(SPtr_Session session, const FBProtocol::CPkt_DeadMonster& pkt)
{    
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// >table CPkt_DeadMonster
	/// >{
	/// >
	/// >}
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	return true;
}

bool FBsPacketFactory::Process_CPkt_RemoveMonster(SPtr_Session session, const FBProtocol::CPkt_RemoveMonster& pkt)
{    
	///> ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///> table CPkt_RemoveMonster
	///> {
	///> 
	///> }
	///> ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	return true;
}

bool FBsPacketFactory::Process_CPkt_Monster_Transform(SPtr_Session session, const FBProtocol::CPkt_Monster_Transform& pkt)
{    
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// >table CPkt_Monster_Transform
	/// >{
	/// >
	/// >}
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	return true;
}

bool FBsPacketFactory::Process_CPkt_Monster_HP(SPtr_Session session, const FBProtocol::CPkt_Monster_HP& pkt)
{    
	///> ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///> table CPkt_Monster_HP
	///> {
	///> 
	///> }
	///> ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	return true;
}

bool FBsPacketFactory::Process_CPkt_Monster_State(SPtr_Session session, const FBProtocol::CPkt_Monster_State& pkt)
{    
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///>table CPkt_Monster_State
	///>{
	///>
	///>}
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	return true;
}

/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ PROCESS [ BULLET ] Client PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

bool FBsPacketFactory::Process_CPkt_Bullet_OnShoot(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnShoot& pkt)
{
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// >table CPkt_Bullet_OnShoot
	/// >{
	/// >
	/// >}
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	int  player_id   = gameSession->GetID(); // 플레이어 아이디
	auto gun_id      = gameSession->GetPlayer()->S_GetCurrWeapon();
	Vec3 ray         = GetVector3(pkt.ray());
	Vec3 firePos     = GetVector3(pkt.pos());


	switch (gun_id)
	{
	case FBProtocol::ITEM_TYPE_WEAPON_MINE_LAUNCHER:
	case FBProtocol::ITEM_TYPE_WEAPON_AIR_STRIKE: {
		// server 에서 simulation 한다. 
		gameSession->GetPlayer()->OnShoot(firePos, ray);
	}
		break;
	default:
		break;
	}
	
	int  bullet_id = {}; // 의미 없어짐. 
	//LOG_MGR->Cout("[", player_id, "]RAY : ", ray.x, " ", ray.y, " ", ray.z, " ---> ", bullet_id, "\n");

	/// 플레이어가 Shot 했다는 것을 플레이어들에게 알린다. 
	auto spkt = FBS_FACTORY->SPkt_Bullet_OnShoot(player_id, gun_id, bullet_id, firePos, ray);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, player_id);

	return true;
}

bool FBsPacketFactory::Process_CPkt_Bullet_OnHitEnemy(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnHitEnemy& pkt)
{
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	int  player_id = gameSession->GetID(); // 플레이어 아이디
	auto gun_id = gameSession->GetPlayer()->S_GetCurrWeapon();
	
	int32_t monster_id	= pkt.monster_id();
	Vec3 ray			= GetVector3(pkt.ray());
	Vec3 fire_pos       = GetVector3(pkt.fire_pos());
	int  bullet_id		= gameSession->GetPlayer()->OnHitEnemy(monster_id, fire_pos, ray); // PQCS -> Bullet Update Start ( Worker Thread  에게 업데이트를 떠넘긴다 ) 

	return true;
}

bool FBsPacketFactory::Process_CPkt_Bullet_OnCollision(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnCollision& pkt)
{   
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// >table CPkt_Bullet_OnCollision
	/// >{
	/// >
	/// >}
	/// >●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	return true;
}

bool FBsPacketFactory::Process_CPkt_Item_Interact(SPtr_Session session, const FBProtocol::CPkt_Item_Interact& pkt)
{
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	SPtr<GamePlayer> player = gameSession->GetPlayer();

	uint32_t				item_id   = pkt.item_id();
	FBProtocol::ITEM_TYPE	item_type = pkt.item_type();

	auto npcC = player->GetOwnerPlayerController()->GetOwnerRoom()->GetNPCController();
	auto item = npcC->GetItem(item_id);
	if (item) {
		item->DoInteract(player);
	}

	return true;
}

bool FBsPacketFactory::Process_CPkt_Item_ThrowAway(SPtr_Session session, const FBProtocol::CPkt_Item_ThrowAway& pkt)
{
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	SPtr<GamePlayer> player = gameSession->GetPlayer();

	uint32_t				item_id = pkt.item_id();
	FBProtocol::ITEM_TYPE	item_type = pkt.item_type();

	auto npcC = player->GetOwnerPlayerController()->GetOwnerRoom()->GetNPCController();
	auto item = npcC->GetItem(item_id);
	if (item) {
		item->DoInteract(player);
	}
	return true;
}


/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ PROCESS [ PHERO ] Client PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

bool FBsPacketFactory::Process_CPkt_GetPhero(SPtr_Session session, const FBProtocol::CPkt_GetPhero& pkt)
{   
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///>table CPkt_GetPhero
	///>{
	///>
	///>}
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●

	return true;
}





/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ SEND SERVER PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★


/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ SEND [ LogIn, Chat, NetworkLatency ] PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

SPtr_SendPktBuf FBsPacketFactory::SPkt_LogIn(bool success)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_LogIn
	/// > {
	///	> 	success: bool;			// 1 byte 
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder;

	/* CREATE LOG IN PACKET */
	auto ServerPacket = FBProtocol::CreateSPkt_LogIn(builder, success);
	builder.Finish(ServerPacket);

	/* Create SendBuffer */
	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;

	return SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_LogIn);
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Chat(uint32_t player_id, std::string msg)
{	
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_Chat
	/// > {
	///	> 	player_id: ulong;		// uint64
	///	> 	message: string;	// 가변 크기
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder;

	auto msgOffset = builder.CreateString(msg);
	auto ServerPacket = FBProtocol::CreateSPkt_Chat(builder, player_id, msgOffset);

	builder.Finish(ServerPacket);

	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBsProtocolID::SPkt_Chat);
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_EnterGame(SPtr<GamePlayer>& myinfo, std::vector<SPtr<GamePlayer>>& players)
{
	flatbuffers::FlatBufferBuilder builder;

	std::vector<flatbuffers::Offset<FBProtocol::Player>> PlayerSnapShots_vector;

	/// +-------------------------------------------------------------------------------------
	///		INTERPRET MY PLAYER INFO 
	/// -------------------------------------------------------------------------------------+	
	auto transSNS          = myinfo.get()->GetTransform()->GetSnapShot(); 
	Vec3 transPos          = transSNS.GetPosition();
	Vec3 transRot          = transSNS.GetRotation();
	Vec3 transSpineLookDir = transSNS.GetLook();

	auto position      = FBProtocol::CreateVector3(builder, transPos.x, transPos.y, transPos.z);
	auto rotation      = FBProtocol::CreateVector3(builder, transRot.x, transRot.y, transRot.z);
	auto transform     = FBProtocol::CreateTransform(builder, position, rotation);
	auto Spine_LookDir = FBProtocol::CreateVector3(builder, 0.f, 0.f, 0.f);
	auto Myinfo        = CreatePlayer(builder, myinfo->GetID(), builder.CreateString(myinfo->GetName()), FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER, transform, Spine_LookDir);


	/// +-------------------------------------------------------------------------------------
	///		INTERPRET REMOTE PLAYERS INFO 
	/// -------------------------------------------------------------------------------------+	
	for (auto& p : players) {
		auto transSNS          = p->GetTransform()->GetSnapShot();
		Vec3 transPos          = transSNS.GetPosition();
		Vec3 transRot          = transSNS.GetRotation();
		Vec3 transSpineLookDir = p->S_GetSpineLookDir();

		auto ID             = p->GetID();
		auto name           = builder.CreateString(p->GetName());
		auto position       = FBProtocol::CreateVector3(builder, transPos.x, transPos.y, transPos.z);
		auto rotation       = FBProtocol::CreateVector3(builder, transRot.x, transRot.y, transRot.z);
		auto transform      = FBProtocol::CreateTransform(builder, position, rotation);
		auto Spine_LookDir  = FBProtocol::CreateVector3(builder, transSpineLookDir.x, transSpineLookDir.y, transSpineLookDir.z);


		auto PlayerSnapShot = CreatePlayer(builder, ID, name, FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER, transform, Spine_LookDir); // CreatePlayerSnapShot는 스키마에 정의된 함수입니다.
		PlayerSnapShots_vector.push_back(PlayerSnapShot);
	}
	auto PlayerSnapShotsOffset = builder.CreateVector(PlayerSnapShots_vector);


	/// +-------------------------------------------------------------------------------------
	///		SEND ENTER_GAME SPKT 
	/// -------------------------------------------------------------------------------------+	
	auto ServerPacket = FBProtocol::CreateSPkt_EnterGame(builder, Myinfo, PlayerSnapShotsOffset);
	builder.Finish(ServerPacket);
	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBsProtocolID::SPkt_EnterGame);

}

SPtr_SendPktBuf FBsPacketFactory::SPkt_NetworkLatency(long long timestamp)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_NetworkLatency
	/// > {
	///	> 	timestamp: long;	// 8 bytes
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder;

	auto ServerPacket = FBProtocol::CreateSPkt_NetworkLatency(builder, timestamp);

	builder.Finish(ServerPacket);

	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBsProtocolID::SPkt_NetworkLatency);
}

/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ SEND [ PLAYER ] PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

SPtr_SendPktBuf FBsPacketFactory::SPkt_NewPlayer(SPtr<GamePlayer>& newPlayer)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_NewPlayer
	/// > {
	///	> 	newplayer: Player; // 새로운 플레이어가 접속했음을 기존의 세션들에게 알린다. 
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};

	auto transSNS			= newPlayer.get()->GetTransform()->GetSnapShot();
	Vec3 transPos			= transSNS.GetPosition();
	Vec3 transRot			= transSNS.GetRotation();
	Vec3 transSpineLookDir	= transSNS.GetLook();

	auto ID					= newPlayer->GetID();
	auto name				= builder.CreateString(newPlayer->GetName());

	auto position			= FBProtocol::CreateVector3(builder, transPos.x, transPos.y, transPos.z);
	auto rotation			= FBProtocol::CreateVector3(builder, transRot.x, transRot.y, transRot.z);
	auto transform			= FBProtocol::CreateTransform(builder, position, rotation);
	auto Spine_LookDir		= FBProtocol::CreateVector3(builder, transSpineLookDir.x, transSpineLookDir.y, transSpineLookDir.z);
	auto PlayerSnapShot		= CreatePlayer(builder, ID, name, FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER, transform, Spine_LookDir); // CreatePlayerSnapShot는 스키마에 정의된 함수입니다.


	auto ServerPacket = FBProtocol::CreateSPkt_NewPlayer(builder, PlayerSnapShot);
	builder.Finish(ServerPacket);
	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_NewPlayer);


	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_RemovePlayer(uint32_t removeSessionID)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_RemovePlayer
	/// > {
	/// > 	player_id: int; // 4 bytes // 삭제할 플레이어의 아이디 
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};

	int32_t id = static_cast<int32_t>(removeSessionID);
	auto ServerPacket = FBProtocol::CreateSPkt_RemovePlayer(builder, id);
	builder.Finish(ServerPacket);

	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_RemovePlayer);

	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_PlayerOnSkill(uint32_t player_id, FBProtocol::PLAYER_SKILL_TYPE skill_type, float phero_amount, int mindcontrol_monster_id)
{
	flatbuffers::FlatBufferBuilder builder{};

	auto ServerPacket = FBProtocol::CreateSPkt_PlayerOnSkill(builder, player_id, skill_type, phero_amount, mindcontrol_monster_id);
	builder.Finish(ServerPacket);

	

	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_PlayerOnSkill);

	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Player_Transform(uint32_t player_id, int32_t move_state, long long latency
	, float velocity, Vec3 movedir, Vec3 pos, Vec3 rot , Vec3 spine_look, float animparam_h, float animparam_v)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_Player_Transform
	/// > {
	/// > 	player_id		: uint;					// 8 bytes				// uint64
	/// > 
	/// > 	move_state		: PLAYER_MOTION_STATE_TYPE; // 1 byte
	/// > 
	/// > 	latency			: long;						// 8 bytes
	/// > 	velocity		: float;					// 4 bytes
	/// > 	movedir			: Vector3;					// 12 bytes (3 * 4 bytes)
	/// > 	trans			: Transform;				// 24 bytes (Vector3 * 2)
	/// > 
	/// > 
	/// > 	spine_look		: Vector3;					// 12 bytes (3 * 4 bytes)
	/// > 	animparam_h		: float;		 			// 4 bytes
	/// > 	animparam_v		: float;	.				// 4 bytes
	/// > 
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};

	auto MoveDirection = FBProtocol::CreateVector3(builder, movedir.x, movedir.y, movedir.z);
	auto position      = FBProtocol::CreateVector3(builder, pos.x, pos.y, pos.z);
	auto rotation      = FBProtocol::CreateVector3(builder, rot.x, rot.y, rot.z);
	auto transform     = FBProtocol::CreateTransform(builder, position, rotation);
	auto Spine_LookDir = FBProtocol::CreateVector3(builder, spine_look.x, spine_look.y, spine_look.z);

	auto ServerPacket = FBProtocol::CreateSPkt_Player_Transform(builder, player_id,
		static_cast<FBProtocol::PLAYER_MOTION_STATE_TYPE>(move_state), latency, velocity, MoveDirection, transform, Spine_LookDir, animparam_h, animparam_v);

	builder.Finish(ServerPacket);


	const uint8_t* bufferPointer      = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer        = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_Player_Transform);
	return sendBuffer;
}


SPtr_SendPktBuf FBsPacketFactory::SPkt_Player_Animation(uint32_t player_id, int anim_upper_idx, int anim_lower_idx, float anim_param_h, float anim_param_v)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_Player_Animation
	/// > {
	/// > 	player_id				: uint;		// 8 bytes
	/// > 
	/// > 	animation_upper_index	: int;		// 4 bytes
	/// > 	animation_lower_index	: int;		// 4 bytes
	/// > 	animation_param_h		: float;	// 4 bytes
	/// > 	animation_param_v		: float;	// 4 bytes
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};

	uint32_t id                    = static_cast<uint32_t>(player_id);
	int32_t animation_upper_index  = static_cast<int32_t>(anim_upper_idx);
	int32_t animation_lower_index  = static_cast<int32_t>(anim_lower_idx);
	float	animation_param_h      = static_cast<float>(anim_param_h);
	float	animation_param_v      = static_cast<float>(anim_param_v);


	auto ServerPacket = FBProtocol::CreateSPkt_Player_Animation(builder, id, animation_upper_index, animation_lower_index, animation_param_h, animation_param_v);
	builder.Finish(ServerPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Player_Animation);

	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Player_Weapon(uint32_t player_id, FBProtocol::ITEM_TYPE weapon_type)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_Player_Weapon
	/// > {
	/// > 	player_id: ulong;			// 8 bytes
	/// > 	weapon_type: WEAPON_TYPE;	// 1 byte
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};

	auto serverPacket = FBProtocol::CreateSPkt_Player_Weapon(builder, player_id, weapon_type);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Player_Weapon);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Player_AimRotation(uint32_t player_id, float aim_rotation, float spine_angle)
{

	flatbuffers::FlatBufferBuilder builder{};

	auto serverPacket = FBProtocol::CreateSPkt_Player_AimRotation(builder, player_id, aim_rotation, spine_angle);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Player_AimRotation);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPKt_Player_State(uint32_t player_id, float hp, float phero, FBProtocol::PLAYER_STATE_TYPE state)
{
	return nullptr;
}


/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ SEND [ MONSTER ] PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

SPtr_SendPktBuf FBsPacketFactory::SPkt_NewMonster(std::vector<SPtr<GameMonster>>& new_monsters)
{
	///  >  ▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽
	/// > table Phero{
	///	> 		id				: uint;
	///	> 		offset_dist		: Position_Vec2;
	/// > }
	///  > ▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽0
	/// > table Monster{
	/// > 		id		:  uint;
	/// > 		type	:  ubyte; // UINT8
	/// > 		pos_2	:  Position_Vec2;
	/// >		rot_y	:  float;
	/// > 		pheros	: string ;
	/// > 
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_NewMonster
	/// > {
	/// > 	new_monsters: [Monster] ;	// 가변 크기 // Created New Monster Infos 
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};
	std::vector<flatbuffers::Offset<FBProtocol::Monster>> MonsterSnapShots_Vector;

	/// +------------------------------------------------------------------------------------------
	///	Monster 정보 저장 
	/// ------------------------------------------------------------------------------------------+
	for (SPtr<GameMonster>& mon : new_monsters) {
		Script_Stat::ObjectState objState = mon->S_GetObjectState();
		if (objState == Script_Stat::ObjectState::Dead ||
			objState == Script_Stat::ObjectState::End) {
			continue;
		}


		auto transSNS	= mon.get()->GetTransform()->GetSnapShot();
		Vec3 transPos	= transSNS.GetPosition();
		Vec3 transRot	= transSNS.GetRotation();
		Vec3 transLook	= transSNS.GetLook();

		auto pos		= FBProtocol::CreatePosition_Vec2(builder, transPos.x, transPos.z);
		auto bt_Type	= mon->GetEnemyController()->GetMonsterBTType(); /* Lock Read */
		float rot_y		= Vector3::SignedAngle(Vector3::Forward, transLook, Vector3::Up);
		auto Monster	= FBProtocol::CreateMonster(builder, mon->GetID(), mon->GetMonsterType(), bt_Type, pos, rot_y);
		MonsterSnapShots_Vector.push_back(Monster);
	}

	auto monsterSnapShots_Offset = builder.CreateVector(MonsterSnapShots_Vector);
	auto serverPacket            = FBProtocol::CreateSPkt_NewMonster(builder, monsterSnapShots_Offset);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer   = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_NewMonster);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_DeadMonster(uint32_t monster_id, Vec3 dead_point, std::string pheros)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_DeadMonster
	/// > {
	/// > 	id			: uint;
	/// > 	dead_point	: Position_Vec2; ( float , float )
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○'
	
	flatbuffers::FlatBufferBuilder builder{};
	auto dead_p       = FBProtocol::CreatePosition_Vec2(builder, dead_point.x, dead_point.z); // x , z ( y아님 )
	auto pheros_str	  = builder.CreateString(pheros);
	auto serverPacket = FBProtocol::CreateSPkt_DeadMonster(builder, monster_id, dead_p, pheros_str);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_DeadMonster);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_RemoveMonster(uint32_t monster_id)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_RemoveMonster
	/// > {
	/// > 	monster_id: int;	// 4 bytes
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	
	flatbuffers::FlatBufferBuilder builder{};

	auto serverPacket = FBProtocol::CreateSPkt_RemoveMonster(builder, monster_id);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_RemoveMonster);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Monster_Transform(uint32_t monster_id, Vec3 pos, Vec3 look)
{
	///> ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	///> table SPkt_Monster_Transform
	///> {
	///> 	monster_id: int;	// 4 bytes
	///> 		pos_2				:  Position_Vec2;
	/// >		rot_y:  float;				// y
	///> }
	///> ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};

	auto PosFBs = FBProtocol::CreatePosition_Vec2(builder, pos.x, pos.z);
	float rot_Y = Vector3::SignedAngle(Vector3::Forward, look, Vector3::Up);

	auto serverPacket = FBProtocol::CreateSPkt_Monster_Transform(builder, monster_id, PosFBs, rot_Y);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Monster_Transform);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Monster_HP(uint32_t monster_id, float hp)
{

	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_Monster_HP
	/// > {
	/// > 	monster_id: int;		// 4 bytes
	/// > 	hp: float;		// 4 bytes
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};

	auto serverPacket = FBProtocol::CreateSPkt_Monster_HP(builder, monster_id, hp);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Monster_HP);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Monster_State(uint32_t monster_id, FBProtocol::MONSTER_BT_TYPE state, int32_t step)
{

	///> ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	///> table SPkt_Monster_State
	///> {
	///> 	monster_id: int;			// 4 bytes
	///> 	state: MONSTER_STATE_TYPE;	// 1 byte
	///> 
	///> }
	///> ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};

	auto serverPacket = FBProtocol::CreateSPkt_Monster_State(builder, monster_id, state, step);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Monster_State);
	return sendBuffer;

}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Monster_Target(uint32_t monster_id, int32_t target_player_id, int32_t target_monster_id)
{
	flatbuffers::FlatBufferBuilder builder{};


	auto serverPacket = FBProtocol::CreateSPkt_MonsterTarget(builder, monster_id, target_player_id, target_monster_id);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Monster_Target);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_GetPhero(uint32_t phero_id, uint32_t player_id)
{
	flatbuffers::FlatBufferBuilder builder{};


	auto serverPacket = FBProtocol::CreateSPkt_GetPhero(builder, phero_id, player_id);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_GetPhero);
	return sendBuffer;

}


/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ SEND [ BULLET ] PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

SPtr_SendPktBuf FBsPacketFactory::SPkt_Bullet_OnShoot(uint32_t player_id, FBProtocol::ITEM_TYPE gun_id, uint32_t bullet_id, Vec3 fire_pos, Vec3 ray)
{

	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_Bullet_OnShoot
	/// > {
	/// > 	player_id	: int; // 4 bytes - 어떤 플레이어가 
	/// > 	gun_id		: int; // 4 bytes - 어떤 총이고 
	/// > 	bullet_id	: int; // 4 bytes - 어떤 총알을 쐈는가
	/// > 
	/// > 	ray			: Vector3; // 12 bytes (4bytes * 3) - 총구 방향은 어떠한가? 
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};

	auto Ray     = FBProtocol::CreateVector3(builder, ray.x, ray.y, ray.z);
	auto firePos = FBProtocol::CreateVector3(builder, fire_pos.x, fire_pos.y, fire_pos.z);

	auto serverPacket = FBProtocol::CreateSPkt_Bullet_OnShoot(builder, player_id, gun_id, bullet_id, firePos, Ray);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Bullet_OnShoot);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Bullet_OnHitEnemy(uint32_t player_id, FBProtocol::ITEM_TYPE gun_id, uint32_t bullet_id, Vec3 ray)
{

	flatbuffers::FlatBufferBuilder builder{};

	auto Ray = FBProtocol::CreateVector3(builder, ray.x, ray.y, ray.z);

	auto serverPacket = FBProtocol::CreateSPkt_Bullet_OnHitEnemy(builder, player_id, gun_id, bullet_id, Ray);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::CPkt_Bullet_OnHitEnemy);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Bullet_OnCollision(uint32_t player_id, FBProtocol::ITEM_TYPE gun_id, uint32_t bullet_id)
{
	///> ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	///> table SPkt_Bullet_OnCollision
	///> {
	///> 	player_id	: int; // 4 bytes - 어떤 플레이어가 
	///> 	gun_id		: int; // 4 bytes - 어떤 총이고 
	///> 	bullet_id	: int; // 4 bytes - 어떤 총알이 충돌했는가?
	///> }
	///> ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder{};


	auto serverPacket = FBProtocol::CreateSPkt_Bullet_OnCollision(builder, player_id, gun_id, bullet_id);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Bullet_OnCollision);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Item_Interact(uint32_t player_id, uint32_t item_id, FBProtocol::ITEM_TYPE item_type, Vec3 drop_pos)
{
	flatbuffers::FlatBufferBuilder builder{};

	auto DropPosition = FBProtocol::CreateVector3(builder, drop_pos.x, drop_pos.y, drop_pos.z);

	auto serverPacket = FBProtocol::CreateSPkt_Item_Interact(builder, player_id, item_id, item_type, DropPosition);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Item_Interact);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Item_ThrowAway(uint32_t player_id, uint32_t item_id, FBProtocol::ITEM_TYPE item_type, Vec3 drop_pos)
{
	flatbuffers::FlatBufferBuilder builder{};

	auto DropPosition = FBProtocol::CreateVector3(builder, drop_pos.x, drop_pos.y, drop_pos.z);

	auto serverPacket = FBProtocol::CreateSPkt_Item_ThrowAway(builder, player_id, item_id, item_type, DropPosition);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Item_ThrowAway);
	return sendBuffer;
}


/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	▶ UTILITY  
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

Vec3 FBsPacketFactory::GetVector3(const FBProtocol::Vector3* vec3)
{
	Vec3 Vector3 = Vec3(vec3->x(), vec3->y(), vec3->z());

	return Vector3;
}

Vec4 FBsPacketFactory::GetVector4(const FBProtocol::Vector4* vec4)
{
	Vec4 Vector4 = Vec4(vec4->x(), vec4->y(), vec4->z(), vec4->w());

	return Vector4;
}

Vec3 FBsPacketFactory::GetPosition_Vec2(float x, float z)
{
	Vec3 Pos_xz = Vec3(x, 0.f, z);
	return Pos_xz;
}

