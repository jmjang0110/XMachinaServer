#include "pch.h"
#include "FBsPacketFactory.h"
#include "Framework.h"

#include "ServerLib/SendBuffersFactory.h"
#include "ServerLib/PacketHeader.h"
#include "ServerLib/SocketData.h"

#include "Contents/GameSession.h"

#include "Contents/RoomManager.h"
#include "Contents/NPCController.h"
#include "Contents/PlayerController.h"
#include "Contents/GameRoom.h"	
#include "Contents/DBController.h"

#include "Contents/GameObject.h"
#include "Contents/Transform.h"
#include "Contents/Component.h"
#include "Contents/Animation.h"
#include "Contents/Collider.h"
#include "Contents/ResourceManager.h"


#include "Contents/Script_EnemyController.h"
#include "Contents/Script_Enemy.h"
#include "Contents/Script_Player.h"
#include "Contents/Script_Weapon.h"
#include "Contents/Script_Item.h"
#include "Contents/Script_SkillMindControl.h"


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
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_LogIn:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_LogIn ]\n");
		const FBProtocol::CPkt_LogIn* packet = flatbuffers::GetRoot<FBProtocol::CPkt_LogIn>(DataPtr);
		if (!packet) return false;
		Process_CPkt_LogIn(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_EnterLobby:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_EnterLobby ]\n");
		const FBProtocol::CPkt_EnterLobby* packet = flatbuffers::GetRoot<FBProtocol::CPkt_EnterLobby>(DataPtr);
		if (!packet) return false;
		Process_CPkt_EnterLobby(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_EnterGame:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_EnterGame ]\n");
		const FBProtocol::CPkt_EnterGame* packet = flatbuffers::GetRoot<FBProtocol::CPkt_EnterGame>(DataPtr);
		if (!packet) return false;
		Process_CPkt_EnterGame(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_PlayGame:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_PlayGame ]\n");
		const FBProtocol::CPkt_PlayGame* packet = flatbuffers::GetRoot<FBProtocol::CPkt_PlayGame>(DataPtr);
		if (!packet) return false;
		Process_CPkt_PlayGame(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Chat:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_Chat ]\n");
		const FBProtocol::CPkt_Chat* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Chat>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Chat(session, *packet);
		break;
	}

	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Custom:
	{
		const FBProtocol::CPkt_Custom* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Custom>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Custom(session, *packet);
		break;
	}

	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_NetworkLatency:
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
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_NewPlayer:
	{
		const FBProtocol::CPkt_NewPlayer* packet = flatbuffers::GetRoot<FBProtocol::CPkt_NewPlayer>(DataPtr);
		if (!packet) return false;
		Process_CPkt_NewPlayer(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_RemovePlayer:
	{
		const FBProtocol::CPkt_RemovePlayer* packet = flatbuffers::GetRoot<FBProtocol::CPkt_RemovePlayer>(DataPtr);
		if (!packet) return false;
		Process_CPkt_RemovePlayer(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Player_Transform:
	{
		const FBProtocol::CPkt_Player_Transform* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_Transform>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_Transform(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Player_Animation:
	{
		const FBProtocol::CPkt_Player_Animation* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_Animation>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_Animation(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Player_Weapon:
	{
		const FBProtocol::CPkt_Player_Weapon* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_Weapon>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_Weapon(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Player_AimRotation:
	{
		const FBProtocol::CPkt_Player_AimRotation* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_AimRotation>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_AimRotation(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_PlayerOnSkill:
	{
		const FBProtocol::CPkt_PlayerOnSkill* packet = flatbuffers::GetRoot<FBProtocol::CPkt_PlayerOnSkill>(DataPtr);
		if (!packet) return false;
		Process_CPkt_PlayerOnSkill(session, *packet);
		break;
	}
	break;
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Player_State:
	{
		const FBProtocol::CPkt_Player_State* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Player_State>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Player_State(session, *packet);
	}
	break;

	/// +--------------------------------
	///		 PROCESS CPKT MONSTER
	/// --------------------------------+
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_NewMonster:
	{
		const FBProtocol::CPkt_NewMonster* packet = flatbuffers::GetRoot<FBProtocol::CPkt_NewMonster>(DataPtr);
		if (!packet) return false;
		Process_CPkt_NewMonster(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_RemoveMonster:
	{
		const FBProtocol::CPkt_RemoveMonster* packet = flatbuffers::GetRoot<FBProtocol::CPkt_RemoveMonster>(DataPtr);
		if (!packet) return false;
		Process_CPkt_RemoveMonster(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Monster_Transform:
	{
		const FBProtocol::CPkt_Monster_Transform* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Monster_Transform>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Monster_Transform(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Monster_HP:
	{
		const FBProtocol::CPkt_Monster_HP* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Monster_HP>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Monster_HP(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Monster_State:
	{
		const FBProtocol::CPkt_Monster_State* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Monster_State>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Monster_State(session, *packet);
		break;
	}


	/// +--------------------------------
	///		 PROCESS CPKT BULLET
	/// --------------------------------+
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Bullet_OnShoot:
	{
		const FBProtocol::CPkt_Bullet_OnShoot* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Bullet_OnShoot>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Bullet_OnShoot(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Bullet_OnHitEnemy:
	{
		const FBProtocol::CPkt_Bullet_OnHitEnemy* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Bullet_OnHitEnemy>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Bullet_OnHitEnemy(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Bullet_OnHitExpEnemy:
	{
		const FBProtocol::CPkt_Bullet_OnHitExpEnemy* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Bullet_OnHitExpEnemy>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Bullet_OnHitExpEnemy(session, *packet);
		break;
	}
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Bullet_OnCollision:
	{
		const FBProtocol::CPkt_Bullet_OnCollision* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Bullet_OnCollision>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Bullet_OnCollision(session, *packet);
		break;
	}
	/// +--------------------------------
	///		 PROCESS CPKT ITEM
	/// --------------------------------+
	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Item_Interact:
	{
		const FBProtocol::CPkt_Item_Interact* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Item_Interact>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Item_Interact(session, *packet);
		break;
	}

	case FBProtocol::FBsProtocolID::FBsProtocolID_CPkt_Item_ThrowAway:
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
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);
	
	std::string ID       = pkt.id()->c_str();
	std::string Password = pkt.password()->c_str();

#ifdef SET_DATA_FROM_DATABASE
	gameSession->LoadUserInfo(ID, Password);
#else
	/* Send Log In Packet */
	auto spkt = FBS_FACTORY->SPkt_LogIn("No DataBase Version", true); 
	session->Send(spkt);
#endif

	LOG_MGR->SetColor(TextColor::BrightBlue);
	LOG_MGR->Cout("LOG IN SESSION ID : ", gameSession->GetID());
	LOG_MGR->WCout(L"-- LOG-IN-IP (Try...) : IPv4-", gameSession->GetSocketData().GetIpAddress().c_str(), '\n');
	LOG_MGR->SetColor(TextColor::Default);

	//gameSession->GetPlayer()->GetTransform()->SetPosition(Vec3(65, 0, 240));
	//gameSession->GetPlayer()->Update();

	/// +--------------------------------
	/// SEND LOG IN PKT TO ME ( SESSION )
	/// --------------------------------+
	/// +---------------------------------------------------------------------------------------------------------------
	//bool LogInSuccess  = true;						   // IS MY GAME PLAYER SUCCESS ?
	/// ---------------------------------------------------------------------------------------------------------------+

	//auto SendPkt_LogIn     = FBS_FACTORY->SPkt_LogIn(LogInSuccess);
	//session->Send(SendPkt_LogIn);
	
	return true;
}

bool FBsPacketFactory::Process_CPkt_EnterLobby(SPtr_Session session, const FBProtocol::CPkt_EnterLobby& pkt)
{
	LOG_MGR->Cout("Process_CPkt_EnterLobby\n");
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	int								ID                = pkt.player_id();
	SPtr<GameObject>				MyPlayer          = gameSession->GetPlayer();
	std::vector<SPtr<GameObject>>	RemotePlayers     = ROOM_MGR->GetAllPlayersInRoom(MyPlayer->GetOwnerRoom()->GetID());
	int								player_EnterOrder = MyPlayer->GetOwnerRoom()->GetPlayerController()->GetPlayersSize();

	gameSession->UpdateUserInfo();
	auto spkt = FBS_FACTORY->SPkt_EnterLobby(player_EnterOrder, MyPlayer, RemotePlayers);
	session->Send(spkt);


#ifdef SERVER_STRESS_TEST

#else
	/// +---------------------------------------------------------------------------------------
	/// SEND NEW PLAYER PKT TO SESSIONS IN ROOM ( SESSION->GET ROOM ID ) - EXCEPT ME ( SESSION )
	/// ---------------------------------------------------------------------------------------+
	auto SendPkt_NewPlayer = FBS_FACTORY->SPkt_NewPlayer(MyPlayer);
	ROOM_MGR->BroadcastRoom(gameSession->GetPlayer()->GetOwnerRoom()->GetID(), SendPkt_NewPlayer, gameSession->GetID());
#endif	
	return true;
}

bool FBsPacketFactory::Process_CPkt_EnterGame(SPtr_Session session, const FBProtocol::CPkt_EnterGame& pkt)
{
    /// ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	/// table CPkt_EnterGame
	/// {
	/// 	player_id: uint;	// 8 bytes
	/// }
	/// ●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●

	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);
	
	int								ID             = pkt.player_id();
	SPtr<GameObject>				MyPlayer       = gameSession->GetPlayer();
	std::vector<SPtr<GameObject>>	RemotePlayers  = ROOM_MGR->GetAllPlayersInRoom(MyPlayer->GetOwnerRoom()->GetID());

	auto SendSPkt_EnterGame = FBS_FACTORY->SPkt_EnterGame(MyPlayer, RemotePlayers);
	session->Send(SendSPkt_EnterGame);

	return true;
}

bool FBsPacketFactory::Process_CPkt_PlayGame(SPtr_Session session, const FBProtocol::CPkt_PlayGame& pkt)
{
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	auto room = gameSession->GetPlayer()->GetOwnerRoom();
	UINT32 player_count = room->GetPlayerController()->GetPlayersSize();

	int minPlayerCountToPlayGame = 500;
	if (player_count >= minPlayerCountToPlayGame) {
		auto spkt = FBS_FACTORY->SPkt_PlayGame();
		ROOM_MGR->BroadcastRoom(room->GetID(), spkt);
		room->SetRoomState(RoomState::Battle);

	}
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
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);
	std::string message = pkt.message()->c_str();
	auto spkt = FBS_FACTORY->SPkt_Chat(session->GetID(), message);
	ROOM_MGR->BroadcastRoom(gameSession->GetPlayer()->GetOwnerRoom()->GetID(), spkt, session->GetID());

	return true;
}

bool FBsPacketFactory::Process_CPkt_Custom(SPtr_Session session, const FBProtocol::CPkt_Custom& pkt)
{
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);
	std::string trooperskin = pkt.trooperskin()->c_str();

	auto spkt = FBS_FACTORY->SPkt_Custom(session->GetID(), trooperskin);
	ROOM_MGR->BroadcastRoom(gameSession->GetPlayer()->GetOwnerRoom()->GetID(), spkt, session->GetID());

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

	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);


	// 패킷으로부터 long long으로 시간을 받음
	long long timestamp = pkt.timestamp();

	/* 클라이언트의 패킷을 그대로 다시 보낸다. */
	auto spkt = FBS_FACTORY->SPkt_NetworkLatency(timestamp);

	session->Send(spkt);
	ROOM_MGR->Send(spkt, gameSession->GetPlayer()->GetOwnerRoom()->GetID(), session->GetID());

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
	SPtr<GameSession> gameSession                          = std::static_pointer_cast<GameSession>(session);
	FBProtocol::PLAYER_SKILL_TYPE	type				   =  pkt.skill_type();
	auto							playerScript           = gameSession->GetPlayer()->GetScriptEntity<Script_Player>();
	float							PheroAmount            = playerScript->GetPhero();
	int								mindcontrol_monster_id = pkt.mindcontrol_monster_id();

	auto player        = gameSession->GetPlayer();
	auto player_entity = gameSession->GetPlayer()->GetScriptEntity<Script_Player>();

	/// +---------------------------------------------------------------------------------------
	/// SEND NEW PLAYER PKT TO SESSIONS IN ROOM ( SESSION->GET ROOM ID ) - EXCEPT ME ( SESSION )
	/// ---------------------------------------------------------------------------------------+
	sptr<GameObject> mindControlMonster = nullptr;
	auto npcController = gameSession->GetPlayer()->GetOwnerRoom()->GetNPCController();
	mindControlMonster = npcController->GetMonster(mindcontrol_monster_id);
	
	if (type == FBProtocol::PLAYER_SKILL_TYPE::PLAYER_SKILL_TYPE_MIND_CONTROL) {
		auto mindcontrol = player_entity->GetSKill(type)->GetScriptEntity<Script_SkillMindControl>();
		mindcontrol->Init(mindControlMonster);
	}

	if (player_entity->GetSkillEntity(type)->GetCurrSkillState() == SkillState::Active) {
		player_entity->GetSkillEntity(type)->SetSkillState(SkillState::CoolTime_Start);
		player_entity->GetSKill(type)->DeActivate();
	}
	else {
		player_entity->OnSkill(type, mindControlMonster);

	}

	auto SPkt = FBS_FACTORY->SPkt_PlayerOnSkill(gameSession->GetPlayer()->GetID(), type, PheroAmount, mindcontrol_monster_id);
	ROOM_MGR->BroadcastRoom(player->GetOwnerRoom()->GetID(), SPkt, gameSession->GetPlayer()->GetID());

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

	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);
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

	long long				move_time	= pkt.move_time(); // 그대로 다시 보낸다. 
	
	/* Boradcast Player's Transform Update */
	SPtr_SendPktBuf SendPkt = FBS_FACTORY->SPkt_Player_Transform(pkt.client_id(), id, move_state, latency, velocity, movedir, pos, rot, spine_look, animparam_h, animparam_v, move_time);
	ROOM_MGR->BroadcastRoom(gameSession->GetPlayer()->GetOwnerRoom()->GetID(), SendPkt, id);


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
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	int		ObjectID                = session->GetID();

	int32_t animation_upper_idx     = pkt.animation_upper_index();
	int32_t animation_lower_idx     = pkt.animation_lower_index();
	float	animation_param_h       = pkt.animation_param_h();
	float	animation_param_v       = pkt.animation_param_v();


	/* 클라이언트의 패킷을 그대로 다시 보낸다. */
	auto spkt = FBS_FACTORY->SPkt_Player_Animation(ObjectID, animation_upper_idx, animation_lower_idx, animation_param_h, animation_param_v);
	ROOM_MGR->BroadcastRoom(gameSession->GetPlayer()->GetOwnerRoom()->GetID(), spkt, ObjectID);

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
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	FBProtocol::ITEM_TYPE	weaponType = pkt.weapon_type();
	int						weapon_id  = pkt.item_id();

	auto player_entity = gameSession->GetPlayerEntity();

	if (weaponType == FBProtocol::ITEM_TYPE_WEAPON_AIR_STRIKE) {
		player_entity->SetAirStrike();
	}
	else {
		auto weapon = player_entity->GetWeapon(weapon_id);
		if (weapon) {
			player_entity->SetWeapon(weapon);
		}
	}

	LOG_MGR->Cout(gameSession->GetID(), " - WEAPON TYPE : ", static_cast<int>(weaponType), "\n");

	auto spkt = FBS_FACTORY->SPkt_Player_Weapon(session->GetID(), weaponType);
	ROOM_MGR->BroadcastRoom(gameSession->GetPlayer()->GetOwnerRoom()->GetID(), spkt, session->GetID());

	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_AimRotation(SPtr_Session session, const FBProtocol::CPkt_Player_AimRotation& pkt)
{
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
	///> table CPkt_Player_AimRotation
	///> {B
	///> 	aim_rotation			: float; // Y rotation Euler
	///> }
	///>●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●

	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	float aim_rotation = pkt.aim_rotation();
	float spine_angle  = pkt.spine_angle();
	auto spkt          = FBS_FACTORY->SPkt_Player_AimRotation(session->GetID(), aim_rotation, spine_angle);

	ROOM_MGR->BroadcastRoom(gameSession->GetPlayer()->GetOwnerRoom()->GetID(), spkt, gameSession->GetID());

	//LOG_MGR->Cout(session->GetID(), " : AIM : ", aim_rotation, '\n');
	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_State(SPtr_Session session, const FBProtocol::CPkt_Player_State& pkt)
{
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);
	
	SPtr<GameObject>			  GameObject   = gameSession->GetPlayer();
	float						  hp           = GameObject->GetScriptEntity<Script_Stat>()->S_GetHp();
	float						  phero        = GameObject->GetScriptEntity<Script_Stat>()->GetPhero();
	FBProtocol::PLAYER_STATE_TYPE state        = pkt.state_type();

	auto spkt = FBS_FACTORY->SPKt_Player_State(session->GetID(), hp, phero, state);
	ROOM_MGR->BroadcastRoom(gameSession->GetPlayer()->GetOwnerRoom()->GetID(), spkt, session->GetID());

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
	
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	int  player_id        = gameSession->GetID(); // 플레이어 아이디
	Vec3 fire_pos         = GetVector3(pkt.fire_pos());
	Vec3 fire_dir         = GetVector3(pkt.fire_dir());
	// On Shoot ! 
	gameSession->GetPlayerEntity()->OnShoot(fire_pos, fire_dir);

	auto weapon   = gameSession->GetPlayerEntity()->GetCurrWeapon();
	auto gun_id   = weapon->GetScriptEntity<Script_Item>()->GetItemType();
	int bullet_id = -1; 

	/// 플레이어가 Shot 했다는 것을 플레이어들에게 알린다. 
	auto spkt = FBS_FACTORY->SPkt_Bullet_OnShoot(player_id, gun_id, bullet_id, fire_pos, fire_dir);
	ROOM_MGR->BroadcastRoom(gameSession->GetPlayer()->GetOwnerRoom()->GetID(), spkt, player_id);

	return true;
}

bool FBsPacketFactory::Process_CPkt_Bullet_OnHitEnemy(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnHitEnemy& pkt)
{
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	int  player_id = gameSession->GetID(); // 플레이어 아이디
	auto equiped_weapon = gameSession->GetPlayer()->GetScriptEntity<Script_Player>()->GetCurrWeapon();
	equiped_weapon->GetScriptEntity<Script_Weapon>();


	int32_t monster_id	= pkt.monster_id();
	Vec3 ray			= GetVector3(pkt.ray());
	Vec3 fire_pos       = GetVector3(pkt.fire_pos());
	int  bullet_id		= gameSession->GetPlayer()->GetScriptEntity<Script_Player>()->OnHitEnemy(monster_id, fire_pos, ray); // PQCS -> Bullet Update Start ( Worker Thread  에게 업데이트를 떠넘긴다 ) 

	return true;
}

bool FBsPacketFactory::Process_CPkt_Bullet_OnHitExpEnemy(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnHitExpEnemy& pkt)
{
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	int monster_id  = pkt.monster_id();
	
	
	gameSession->GetPlayer()->GetScriptEntity<Script_Player>()->OnHitExpEnemy(monster_id); // PQCS -> Bullet Update Start ( Worker Thread  에게 업데이트를 떠넘긴다 ) 


	return false;
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
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);
	SPtr<GameObject> player = gameSession->GetPlayer();

	uint32_t				item_id   = pkt.item_id();
	FBProtocol::ITEM_TYPE	item_type = pkt.item_type();

	LOG_MGR->Cout(item_id, " ITEM ID Do Interact \n");

	auto npcC = player->GetOwnerRoom()->GetNPCController();
	auto item = npcC->GetItem(item_id);
	auto item_entity = item->GetScriptEntity<Script_Item>();
	if (item_entity) {
		item_entity->DoInteract(player);
	}

	return true;
}

bool FBsPacketFactory::Process_CPkt_Item_ThrowAway(SPtr_Session session, const FBProtocol::CPkt_Item_ThrowAway& pkt)
{
	SPtr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);
	SPtr<GameObject> player = gameSession->GetPlayer();

	uint32_t				item_id = pkt.item_id();
	FBProtocol::ITEM_TYPE	item_type = pkt.item_type();
	LOG_MGR->Cout(item_id, " ITEM ID Do ThrowAway \n");

	auto npcC = player->GetOwnerRoom()->GetNPCController();
	auto item = npcC->GetItem(item_id);
	auto item_entity = item->GetScriptEntity<Script_Item>();
	if (item_entity) {
		item_entity->ThrowAway(player);

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

SPtr_SendPktBuf FBsPacketFactory::SPkt_LogIn(std::string name, bool success)
{
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
	/// > table SPkt_LogIn
	/// > {
	///	> 	success: bool;			// 1 byte 
	/// > }
	/// > ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○

	flatbuffers::FlatBufferBuilder builder;

	/* CREATE LOG IN PACKET */
	auto nameOffset   = builder.CreateString(name);
	auto ServerPacket = FBProtocol::CreateSPkt_LogIn(builder, nameOffset, success);
	builder.Finish(ServerPacket);

	/* Create SendBuffer */
	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	
	return SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_LogIn);
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

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Chat);
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Custom(int player_id, std::string trooperName)
{
	flatbuffers::FlatBufferBuilder builder;

	auto msgOffset = builder.CreateString(trooperName);
	auto ServerPacket = FBProtocol::CreateSPkt_Custom(builder, player_id, msgOffset);

	builder.Finish(ServerPacket);

	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Custom);
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_EnterGame(SPtr<GameObject>& myinfo, std::vector<SPtr<GameObject>>& players)
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

#ifdef  SERVER_STRESS_TEST
	auto PlayerSnapShotsOffset = 0;
#else 
	/// +-------------------------------------------------------------------------------------
	///		INTERPRET REMOTE PLAYERS INFO 
	/// -------------------------------------------------------------------------------------+	
	for (auto& p : players) {
		auto transSNS          = p->GetTransform()->GetSnapShot();
		Vec3 transPos          = transSNS.GetPosition();
		Vec3 transRot          = transSNS.GetRotation();
		Vec3 transSpineLookDir = p->GetTransform()->GetLook(); // ??? 

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


#endif //  SERVER_STRESS_TEST



	/// +-------------------------------------------------------------------------------------
	///		SEND ENTER_GAME SPKT 
	/// -------------------------------------------------------------------------------------+	
	auto ServerPacket = FBProtocol::CreateSPkt_EnterGame(builder, Myinfo, PlayerSnapShotsOffset);
	builder.Finish(ServerPacket);
	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_EnterGame);

}

SPtr_SendPktBuf FBsPacketFactory::SPkt_EnterLobby(int player_EnterOrder/*입장 순서*/, SPtr<GameObject>& myinfo, std::vector<SPtr<GameObject>>& players)
{
	flatbuffers::FlatBufferBuilder builder;

	std::vector<flatbuffers::Offset<FBProtocol::Player>> PlayerSnapShots_vector;

	/// +-------------------------------------------------------------------------------------
	///		INTERPRET MY PLAYER INFO 
	/// -------------------------------------------------------------------------------------+	
	auto transSNS			= myinfo.get()->GetTransform()->GetSnapShot(); 
	Vec3 transPos			= transSNS.GetPosition();
	Vec3 transRot			= transSNS.GetRotation();
	Vec3 transSpineLookDir	= transSNS.GetLook();

	auto position			= FBProtocol::CreateVector3(builder, transPos.x, transPos.y, transPos.z);
	auto rotation			= FBProtocol::CreateVector3(builder, transRot.x, transRot.y, transRot.z);
	auto transform			= FBProtocol::CreateTransform(builder, position, rotation);
	auto Spine_LookDir		= FBProtocol::CreateVector3(builder, 0.f, 0.f, 0.f);
	auto Myinfo				= CreatePlayer(builder, myinfo->GetID(), builder.CreateString(myinfo->GetName()), FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER, transform, Spine_LookDir);

#ifdef SERVER_STRESS_TEST
	auto PlayerSnapShotsOffset = 0;
#else 
	/// +-------------------------------------------------------------------------------------
	///		INTERPRET REMOTE PLAYERS INFO 
	/// -------------------------------------------------------------------------------------+	
	for (auto& p : players) {
		if (p->GetName() == "") {
			continue;
		}
		auto transSNS          = p->GetTransform()->GetSnapShot();
		Vec3 transPos          = transSNS.GetPosition();
		Vec3 transRot          = transSNS.GetRotation();
		Vec3 transSpineLookDir = p->GetTransform()->GetLook(); // ??? 

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
#endif

	/// +-------------------------------------------------------------------------------------
	///		SEND ENTER_GAME SPKT 
	/// -------------------------------------------------------------------------------------+	
	auto ServerPacket = FBProtocol::CreateSPkt_EnterLobby(builder, player_EnterOrder, Myinfo, PlayerSnapShotsOffset);
	builder.Finish(ServerPacket);
	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_EnterLobby);
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_PlayGame()
{
	flatbuffers::FlatBufferBuilder builder;
	auto ServerPacket = FBProtocol::CreateSPkt_PlayGame(builder);
	builder.Finish(ServerPacket);
	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_PlayGame);
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

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_NetworkLatency);
}

/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ SEND [ PLAYER ] PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

SPtr_SendPktBuf FBsPacketFactory::SPkt_NewPlayer(SPtr<GameObject>& newPlayer)
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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_NewPlayer);


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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_RemovePlayer);

	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_PlayerOnSkill(uint32_t player_id, FBProtocol::PLAYER_SKILL_TYPE skill_type, float phero_amount, int mindcontrol_monster_id)
{
	flatbuffers::FlatBufferBuilder builder{};

	auto ServerPacket = FBProtocol::CreateSPkt_PlayerOnSkill(builder, player_id, skill_type, phero_amount, mindcontrol_monster_id);
	builder.Finish(ServerPacket);

	

	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_PlayerOnSkill);
	
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Player_Transform(uint32_t client_id, uint32_t player_id, int32_t move_state, long long latency
	, float velocity, Vec3 movedir, Vec3 pos, Vec3 rot , Vec3 spine_look, float animparam_h, float animparam_v, long long move_time)
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

	auto ServerPacket = FBProtocol::CreateSPkt_Player_Transform(builder, client_id, player_id,
		static_cast<FBProtocol::PLAYER_MOTION_STATE_TYPE>(move_state), latency, velocity, MoveDirection, transform, Spine_LookDir, animparam_h, animparam_v, move_time);

	builder.Finish(ServerPacket);


	const uint8_t* bufferPointer      = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer        = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Player_Transform);
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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Player_Animation);

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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Player_Weapon);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Player_AimRotation(uint32_t player_id, float aim_rotation, float spine_angle)
{

	flatbuffers::FlatBufferBuilder builder{};

	auto serverPacket = FBProtocol::CreateSPkt_Player_AimRotation(builder, player_id, aim_rotation, spine_angle);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Player_AimRotation);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPKt_Player_State(uint32_t player_id, float hp, float phero, FBProtocol::PLAYER_STATE_TYPE state)
{
	return nullptr;
}


/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ SEND [ MONSTER ] PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★

SPtr_SendPktBuf FBsPacketFactory::SPkt_NewMonster(std::vector<SPtr<GameObject>>& new_monsters)
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
	for (SPtr<GameObject>& mon : new_monsters) {
		Script_Stat::ObjectState objState = mon->GetScriptEntity<Script_Stat>()->S_GetObjectState();
		if (objState == Script_Stat::ObjectState::Dead ||
			objState == Script_Stat::ObjectState::End) {
			continue;
		}


		auto transSNS	= mon.get()->GetTransform()->GetSnapShot();
		Vec3 transPos	= transSNS.GetPosition();
		Vec3 transRot	= transSNS.GetRotation();
		Vec3 transLook	= transSNS.GetLook();

		auto pos		= FBProtocol::CreatePosition_Vec2(builder, transPos.x, transPos.z);
		auto bt_Type	= mon->GetScript<Script_EnemyController>()->GetMonsterBTType(); /* Lock Read */
		float rot_y		= Vector3::SignedAngle(Vector3::Forward, transLook, Vector3::Up);
		auto Monster	= FBProtocol::CreateMonster(builder, mon->GetID(), mon->GetScriptEntity<Script_Enemy>()->GetMonsterType(), bt_Type, pos, rot_y);
		MonsterSnapShots_Vector.push_back(Monster);
	}

	auto monsterSnapShots_Offset = builder.CreateVector(MonsterSnapShots_Vector);
	auto serverPacket            = FBProtocol::CreateSPkt_NewMonster(builder, monsterSnapShots_Offset);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer   = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_NewMonster);
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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_DeadMonster);
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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_RemoveMonster);
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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Monster_Transform);
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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Monster_HP);
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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Monster_State);
	return sendBuffer;

}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Monster_Target(uint32_t monster_id, int32_t target_player_id, int32_t target_monster_id)
{
	flatbuffers::FlatBufferBuilder builder{};


	auto serverPacket = FBProtocol::CreateSPkt_MonsterTarget(builder, monster_id, target_player_id, target_monster_id);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Monster_Target);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_GetPhero(uint32_t phero_id, uint32_t player_id)
{
	flatbuffers::FlatBufferBuilder builder{};


	auto serverPacket = FBProtocol::CreateSPkt_GetPhero(builder, phero_id, player_id);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_GetPhero);
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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Bullet_OnShoot);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Bullet_OnHitEnemy(uint32_t player_id, FBProtocol::ITEM_TYPE gun_id, uint32_t bullet_id, Vec3 ray)
{

	flatbuffers::FlatBufferBuilder builder{};

	auto Ray = FBProtocol::CreateVector3(builder, ray.x, ray.y, ray.z);

	auto serverPacket = FBProtocol::CreateSPkt_Bullet_OnHitEnemy(builder, player_id, gun_id, bullet_id, Ray);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Bullet_OnHitEnemy);
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
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Bullet_OnCollision);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Item_Interact(uint32_t player_id, uint32_t item_id, FBProtocol::ITEM_TYPE item_type, Vec3 drop_pos)
{
	flatbuffers::FlatBufferBuilder builder{};

	auto DropPosition = FBProtocol::CreateVector3(builder, drop_pos.x, drop_pos.y, drop_pos.z);

	auto serverPacket = FBProtocol::CreateSPkt_Item_Interact(builder, player_id, item_id, item_type, DropPosition);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Item_Interact);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Item_ThrowAway(uint32_t player_id, uint32_t item_id, FBProtocol::ITEM_TYPE item_type, Vec3 drop_pos)
{
	flatbuffers::FlatBufferBuilder builder{};

	auto DropPosition = FBProtocol::CreateVector3(builder, drop_pos.x, drop_pos.y, drop_pos.z);

	auto serverPacket = FBProtocol::CreateSPkt_Item_ThrowAway(builder, player_id, item_id, item_type, DropPosition);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBProtocol::FBsProtocolID::FBsProtocolID_SPkt_Item_ThrowAway);
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

