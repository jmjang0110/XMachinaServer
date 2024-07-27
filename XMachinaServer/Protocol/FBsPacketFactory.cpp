#include "pch.h"
#include "FBsPacketFactory.h"
#include "ServerLib/PacketHeader.h"

#include "../Framework.h"
#include "../ServerLib/SendBuffersFactory.h"
#include "../Contents/GameSession.h"
#include "../ServerLib/SocketData.h"
#include "Contents/GamePlayer.h"
#include "Contents/GameManager.h"
#include "Contents/Skill.h"
#include "Contents/Collider.h"
#include "Contents/Script_Player.h"

DEFINE_SINGLETON(FBsPacketFactory);

/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� PROCESS CLIENT PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��
bool FBsPacketFactory::ProcessFBsPacket(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen)
{
	/* �� Packet Struct ------------------------------------------------- */
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
	case FBsProtocolID::CPkt_Bullet_OnShoot:
	{
		const FBProtocol::CPkt_Bullet_OnShoot* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Bullet_OnShoot>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Bullet_OnShoot(session, *packet);
		break;
	}
	case FBsProtocolID::CPkt_Bullet_OnCollision:
	{
		const FBProtocol::CPkt_Bullet_OnCollision* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Bullet_OnCollision>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Bullet_OnCollision(session, *packet);
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
	return false;
}


/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� PROCESS [LogIn, Chat, NetworkLatency, Entergame] Client PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��
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

	gameSession->GetPlayer()->SetPosition(Vec3(65, 0, 240));

	LOG_MGR->SetColor(TextColor::BrightBlue);
	LOG_MGR->Cout("LOG IN SESSION ID : ", gameSession->GetID());
	LOG_MGR->WCout(L"-- LOG-IN-IP : IPv4-", gameSession->GetSocketData().GetIpAddress().c_str(), '\n');
	LOG_MGR->SetColor(TextColor::Default);


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
    /// �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///table CPkt_EnterGame
	///{
	///	player_id: uint;	// 8 bytes
	///}
	/// �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	int ID = pkt.player_id();

	PlayerSnapShot				MyInfo            = gameSession->GetPlayerSnapShot();  // MY GAME PLAYER INFO 
	std::vector<PlayerSnapShot> RemotePlayersInfo = GAME_MGR->GetPlayerSnapShots_Room(gameSession->GetPlayerSnapShot().RoomID); // REMOTE PLAYERS INFO IN ROOM ( MY PLAYER ROOM ID )

	auto SendSPkt_EnterGame = FBS_FACTORY->SPkt_EnterGame(MyInfo, RemotePlayersInfo);
	session->Send(SendSPkt_EnterGame);

	/// +---------------------------------------------------------------------------------------
	/// SEND NEW PLAYER PKT TO SESSIONS IN ROOM ( SESSION->GET ROOM ID ) - EXCEPT ME ( SESSION )
	/// ---------------------------------------------------------------------------------------+
	auto SendPkt_NewPlayer = FBS_FACTORY->SPkt_NewPlayer(MyInfo);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayerSnapShot().RoomID, SendPkt_NewPlayer, gameSession->GetID());

	/* TEST MONSTER */

	//std::vector<MonsterSnapShot> MonstersInfos;

	//MonsterSnapShot newMon;
	//newMon.ID = 1;
	//newMon.HP = 100;
	//newMon.Position = Vec3(28, 0, 258);
	//newMon.Type = MonsterType::AdvancedCombatDroid_5;

	//MonstersInfos.push_back(newMon);

	//newMon.ID = 2;
	//newMon.HP = 100;
	//newMon.Position = Vec3(40, 0, 258);
	//newMon.Type = MonsterType::Onyscidus;

	//MonstersInfos.push_back(newMon);

	//newMon.ID = 3;
	//newMon.HP = 100;
	//newMon.Position = Vec3(50, 0, 258);
	//newMon.Type = MonsterType::Ursacetus;

	//MonstersInfos.push_back(newMon);


	//std::cout << "BROADCAST NEW MONSTER INFOS \n";

	//auto SendPkt_NewMonster = FBS_FACTORY->SPkt_NewMonster(MonstersInfos);
	//GAME_MGR->BroadcastRoom(gameSession->GetPlayerSnapShot().RoomID, SendPkt_NewMonster);


	return true;
}

bool FBsPacketFactory::Process_CPkt_Chat(SPtr_Session session, const FBProtocol::CPkt_Chat& pkt)
{
	///> �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///> table CPkt_Chat
	///> {
	///>	message: string;	// ���� ũ��
	///> }
	///> �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�

	std::string message = pkt.message()->c_str();

	std::cout << "CPKT CHAT [" << session->GetID() << "] - SESSION : " << session.get() << " DATA : " <<
		message << std::endl;

	return true;
}

bool FBsPacketFactory::Process_CPkt_NetworkLatency(SPtr_Session session, const FBProtocol::CPkt_NetworkLatency& pkt)
{
	///>  �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///> table CPkt_NetworkLatency
	///> {
	///> 	 timestamp: long;	// 8 bytes
	///> }
	///>  �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);


	// ��Ŷ���κ��� long long���� �ð��� ����
	long long timestamp = pkt.timestamp();

	/* Ŭ���̾�Ʈ�� ��Ŷ�� �״�� �ٽ� ������. */
	auto spkt = FBS_FACTORY->SPkt_NetworkLatency(timestamp);

	session->Send(spkt);
	GAME_MGR->Send(spkt, gameSession->GetPlayerSnapShot().RoomID, session->GetID());

	return true;
}


/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� PROCESS [ PLAYER ] Client PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��

bool FBsPacketFactory::Process_CPkt_NewPlayer(SPtr_Session session, const FBProtocol::CPkt_NewPlayer& pkt)
{   
	/// > �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	/// > table CPkt_NewPlayer
	/// > {
	/// > 
	/// > }
	/// > �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	return true;
}

bool FBsPacketFactory::Process_CPkt_RemovePlayer(SPtr_Session session, const FBProtocol::CPkt_RemovePlayer& pkt)
{   /// > �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	/// > table CPkt_RemovePlayer
	/// > {
	/// > 
	/// > }
	/// > �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	return false;
}

bool FBsPacketFactory::Process_CPkt_PlayerOnSkill(SPtr_Session session, const FBProtocol::CPkt_PlayerOnSkill& pkt)
{   
	///> �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///> table CPkt_PlayerOnSkill
	///> {
	///> 	skill_type: PLAYER_SKILL_TYPE;
	///> 
	///> }
	///> �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	FBProtocol::PLAYER_SKILL_TYPE	type				=  pkt.skill_type();
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	auto playerScript = gameSession->GetPlayer()->GetScript<Script_Player>(ScriptInfo::Type::Stat);
	float PheroAmount = playerScript->GetPhero();

	/// +---------------------------------------------------------------------------------------
	/// SEND NEW PLAYER PKT TO SESSIONS IN ROOM ( SESSION->GET ROOM ID ) - EXCEPT ME ( SESSION )
	/// ---------------------------------------------------------------------------------------+
	auto spkt = FBS_FACTORY->SPkt_PlayerOnSkill(session->GetID(), type, PheroAmount);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayerSnapShot().RoomID, spkt, gameSession->GetID());

	LOG_MGR->Cout(session->GetID(), "SKILLON", static_cast<int>(type), "\n");

	return false;
}

bool FBsPacketFactory::Process_CPkt_Player_Transform(SPtr_Session session, const FBProtocol::CPkt_Player_Transform& pkt)
{    
	/// > �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
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
	/// > �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	UINT32 id = session->GetID();

	int32_t					move_state  = pkt.move_state();
									    
	long long				latency     = pkt.latency();			/* �ش� Ŭ���̾�Ʈ�� ��� Latency (ms)  */
	float					velocity    = pkt.velocity();
	Vec3					movedir     = GetVector3(pkt.movedir());
	Vec3					pos         = GetVector3(pkt.trans()->position());
	Vec3					rot         = GetVector3(pkt.trans()->rotation());
	
	Vec3					spine_look  = GetVector3(pkt.spine_look());
	float					animparam_h = pkt.animparam_h();
	float					animparam_v = pkt.animparam_v();

	/* Boradcast Player's Transform Update */
	SPtr_SendPktBuf SendPkt = FBS_FACTORY->SPkt_Player_Transform(id, move_state, latency, velocity, movedir, pos, rot, spine_look, animparam_h, animparam_v);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayerSnapShot().RoomID, SendPkt, id);


	/* Player Info Update */
	gameSession->GetPlayer()->SetPosition(pos);
	gameSession->GetPlayer()->GetTransform()->SetPosition(pos);
	gameSession->GetPlayer()->GetTransform()->SetLocalRotation(Quaternion::ToQuaternion(rot));
	gameSession->GetPlayer()->Update();


	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_Animation(SPtr_Session session, const FBProtocol::CPkt_Player_Animation& pkt)
{   
	/// �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	/// > table CPkt_Player_Animation
	/// > {
	/// > 	animation_upper_index	: int;		// 4 bytes
	/// > 	animation_lower_index	: int;		// 4 bytes
	/// > 	animation_param_h		: float;	// 4 bytes
	/// > 	animation_param_v		: float;	// 4 bytes
	/// > }
	/// �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	int		ObjectID                = session->GetID();

	int32_t animation_upper_idx     = pkt.animation_upper_index();
	int32_t animation_lower_idx     = pkt.animation_lower_index();
	float	animation_param_h       = pkt.animation_param_h();
	float	animation_param_v       = pkt.animation_param_v();


	/* Ŭ���̾�Ʈ�� ��Ŷ�� �״�� �ٽ� ������. */
	auto spkt = FBS_FACTORY->SPkt_Player_Animation(ObjectID, animation_upper_idx, animation_lower_idx, animation_param_h, animation_param_v);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, ObjectID);

	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_Weapon(SPtr_Session session, const FBProtocol::CPkt_Player_Weapon& pkt)
{   
	///>�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///> table CPkt_Player_Weapon
	///> {
	///> 	weapon_type: WEAPON_TYPE;	// 1 byte
	///> }
	///>�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	FBProtocol::WEAPON_TYPE weaponType = pkt.weapon_type();

	LOG_MGR->Cout("WEAPONTYPE : ", static_cast<int>(weaponType));


	auto spkt = FBS_FACTORY->SPkt_Player_Weapon(session->GetID(), weaponType);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, session->GetID());

	return true;
}

bool FBsPacketFactory::Process_CPkt_Player_AimRotation(SPtr_Session session, const FBProtocol::CPkt_Player_AimRotation& pkt)
{
	///>�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///> table CPkt_Player_AimRotation
	///> {
	///> 	aim_rotation			: float; // Y rotation Euler
	///> }
	///>�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	float aim_rotation = pkt.aim_rotation();
	float spine_angle  = pkt.spine_angle();
	auto spkt          = FBS_FACTORY->SPkt_Player_AimRotation(session->GetID(), aim_rotation, spine_angle);

	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, gameSession->GetID());

	LOG_MGR->Cout(session->GetID(), " : AIM : ", aim_rotation, '\n');
	return true;
}


/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� PROCESS [ MONSTER ] Client PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��

bool FBsPacketFactory::Process_CPkt_NewMonster(SPtr_Session session, const FBProtocol::CPkt_NewMonster& pkt)
{   
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	/// >table CPkt_NewMonster
	/// >{
	/// >
	/// >}
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	return false;
}

bool FBsPacketFactory::Process_CPkt_DeadMonster(SPtr_Session session, const FBProtocol::CPkt_DeadMonster& pkt)
{    
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	/// >table CPkt_DeadMonster
	/// >{
	/// >
	/// >}
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	return false;
}

bool FBsPacketFactory::Process_CPkt_RemoveMonster(SPtr_Session session, const FBProtocol::CPkt_RemoveMonster& pkt)
{    
	///> �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///> table CPkt_RemoveMonster
	///> {
	///> 
	///> }
	///> �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	return false;
}

bool FBsPacketFactory::Process_CPkt_Monster_Transform(SPtr_Session session, const FBProtocol::CPkt_Monster_Transform& pkt)
{    
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	/// >table CPkt_Monster_Transform
	/// >{
	/// >
	/// >}
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	return false;
}

bool FBsPacketFactory::Process_CPkt_Monster_HP(SPtr_Session session, const FBProtocol::CPkt_Monster_HP& pkt)
{    
	///> �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///> table CPkt_Monster_HP
	///> {
	///> 
	///> }
	///> �ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	return false;
}

bool FBsPacketFactory::Process_CPkt_Monster_State(SPtr_Session session, const FBProtocol::CPkt_Monster_State& pkt)
{    
	///>�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///>table CPkt_Monster_State
	///>{
	///>
	///>}
	///>�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	return false;
}

/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� PROCESS [ BULLET ] Client PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��

bool FBsPacketFactory::Process_CPkt_Bullet_OnShoot(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnShoot& pkt)
{
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	/// >table CPkt_Bullet_OnShoot
	/// >{
	/// >
	/// >}
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	

	int  player_id   = gameSession->GetID(); // �÷��̾� ���̵�
	auto gun_id      = gameSession->GetPlayer()->GetCurrWeapon();
	Vec3 ray         = GetVector3(pkt.ray());
	int  bullet_id   = gameSession->GetPlayer()->OnShoot(); // PQCS -> Bullet Update Start ( Worker Thread  ���� ������Ʈ�� ���ѱ�� ) 
	
	LOG_MGR->Cout("[", player_id, "]RAY : ", ray.x, " ", ray.y, " ", ray.z, "\n");


	// Shot �Ұ��� 
	//if (bullet_id == -1) {
	//	return false;
	//}

	/// �÷��̾ Shot �ߴٴ� ���� �÷��̾�鿡�� �˸���. 
	auto spkt = FBS_FACTORY->SPkt_Bullet_OnShoot(player_id, gun_id, bullet_id, ray);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayer()->GetRoomID(), spkt, player_id);

	return false;
}

bool FBsPacketFactory::Process_CPkt_Bullet_OnCollision(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnCollision& pkt)
{   
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	/// >table CPkt_Bullet_OnCollision
	/// >{
	/// >
	/// >}
	/// >�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	return false;
}


/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� PROCESS [ PHERO ] Client PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��

bool FBsPacketFactory::Process_CPkt_GetPhero(SPtr_Session session, const FBProtocol::CPkt_GetPhero& pkt)
{   
	///>�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	///>table CPkt_GetPhero
	///>{
	///>
	///>}
	///>�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�

	return false;
}





/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� SEND SERVER PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��


/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� SEND [ LogIn, Chat, NetworkLatency ] PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��

SPtr_SendPktBuf FBsPacketFactory::SPkt_LogIn(bool success)
{
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_LogIn
	/// > {
	///	> 	success: bool;			// 1 byte 
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

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
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_Chat
	/// > {
	///	> 	player_id: ulong;		// uint64
	///	> 	message: string;	// ���� ũ��
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder;

	auto msgOffset = builder.CreateString(msg);
	auto ServerPacket = FBProtocol::CreateSPkt_Chat(builder, player_id, msgOffset);

	builder.Finish(ServerPacket);

	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBsProtocolID::SPkt_Chat);
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_EnterGame(PlayerSnapShot& myinfo, std::vector<PlayerSnapShot>& players)
{
	flatbuffers::FlatBufferBuilder builder;

	std::vector<flatbuffers::Offset<FBProtocol::Player>> PlayerSnapShots_vector;

	/* My Player Info */
	auto position      = FBProtocol::CreateVector3(builder, myinfo.Position.x, myinfo.Position.y, myinfo.Position.z);
	auto rotation      = FBProtocol::CreateVector3(builder, myinfo.Rotation.x, myinfo.Rotation.y, myinfo.Rotation.z);
	auto transform     = FBProtocol::CreateTransform(builder, position, rotation);
	auto Spine_LookDir = FBProtocol::CreateVector3(builder, myinfo.SpineDir.x, myinfo.SpineDir.y, myinfo.SpineDir.z);
	auto Myinfo        = CreatePlayer(builder, myinfo.ID, builder.CreateString(myinfo.Name), FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER, transform, Spine_LookDir);

	/* Remote Players */
	for (PlayerSnapShot& p : players) {
		auto ID             = p.ID;
		auto name           = builder.CreateString(p.Name);
		auto position       = FBProtocol::CreateVector3(builder, p.Position.x, p.Position.y, p.Position.z);
		auto rotation       = FBProtocol::CreateVector3(builder, p.Rotation.x, p.Rotation.y, p.Rotation.z);
		auto transform      = FBProtocol::CreateTransform(builder, position, rotation);
		auto Spine_LookDir  = FBProtocol::CreateVector3(builder, p.SpineDir.x, p.SpineDir.y, p.SpineDir.z);


		auto PlayerSnapShot = CreatePlayer(builder, ID, name, FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER, transform, Spine_LookDir); // CreatePlayerSnapShot�� ��Ű���� ���ǵ� �Լ��Դϴ�.
		PlayerSnapShots_vector.push_back(PlayerSnapShot);
	}
	auto PlayerSnapShotsOffset = builder.CreateVector(PlayerSnapShots_vector);

	/* Enter Game Server Packet */
	auto ServerPacket = FBProtocol::CreateSPkt_EnterGame(builder, Myinfo, PlayerSnapShotsOffset);

	builder.Finish(ServerPacket);

	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBsProtocolID::SPkt_EnterGame);

}

SPtr_SendPktBuf FBsPacketFactory::SPkt_NetworkLatency(long long timestamp)
{
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_NetworkLatency
	/// > {
	///	> 	timestamp: long;	// 8 bytes
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder;

	auto ServerPacket = FBProtocol::CreateSPkt_NetworkLatency(builder, timestamp);

	builder.Finish(ServerPacket);

	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBsProtocolID::SPkt_NetworkLatency);
}

/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� SEND [ PLAYER ] PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��

SPtr_SendPktBuf FBsPacketFactory::SPkt_NewPlayer(PlayerSnapShot& newPlayerSnapShot)
{
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_NewPlayer
	/// > {
	///	> 	newplayer: Player; // ���ο� �÷��̾ ���������� ������ ���ǵ鿡�� �˸���. 
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder{};

	auto ID = newPlayerSnapShot.ID;
	auto name = builder.CreateString(newPlayerSnapShot.Name);

	auto position = FBProtocol::CreateVector3(builder, newPlayerSnapShot.Position.x, newPlayerSnapShot.Position.y, newPlayerSnapShot.Position.z);
	auto rotation = FBProtocol::CreateVector3(builder, newPlayerSnapShot.Rotation.x, newPlayerSnapShot.Rotation.y, newPlayerSnapShot.Rotation.z);
	auto transform = FBProtocol::CreateTransform(builder, position, rotation);
	auto Spine_LookDir = FBProtocol::CreateVector3(builder, newPlayerSnapShot.SpineDir.x, newPlayerSnapShot.SpineDir.y, newPlayerSnapShot.SpineDir.z);
	auto PlayerSnapShot = CreatePlayer(builder, ID, name, FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER, transform, Spine_LookDir); // CreatePlayerSnapShot�� ��Ű���� ���ǵ� �Լ��Դϴ�.

	auto ServerPacket = FBProtocol::CreateSPkt_NewPlayer(builder, PlayerSnapShot);
	builder.Finish(ServerPacket);

	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_NewPlayer);


	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_RemovePlayer(uint32_t removeSessionID)
{
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_RemovePlayer
	/// > {
	/// > 	player_id: int; // 4 bytes // ������ �÷��̾��� ���̵� 
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder{};

	int32_t id = static_cast<int32_t>(removeSessionID);
	auto ServerPacket = FBProtocol::CreateSPkt_RemovePlayer(builder, id);
	builder.Finish(ServerPacket);

	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_RemovePlayer);

	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_PlayerOnSkill(uint32_t player_id, FBProtocol::PLAYER_SKILL_TYPE skill_type, float phero_amount)
{
	flatbuffers::FlatBufferBuilder builder{};

	int32_t id = player_id;
	auto ServerPacket = FBProtocol::CreateSPkt_PlayerOnSkill(builder, id, skill_type);
	builder.Finish(ServerPacket);

	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_PlayerOnSkill);

	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Player_Transform(uint32_t player_id, int32_t move_state, long long latency
	, float velocity, Vec3 movedir, Vec3 pos, Vec3 rot , Vec3 spine_look, float animparam_h, float animparam_v)
{
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
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
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

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
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_Player_Animation
	/// > {
	/// > 	player_id				: uint;		// 8 bytes
	/// > 
	/// > 	animation_upper_index	: int;		// 4 bytes
	/// > 	animation_lower_index	: int;		// 4 bytes
	/// > 	animation_param_h		: float;	// 4 bytes
	/// > 	animation_param_v		: float;	// 4 bytes
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

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

SPtr_SendPktBuf FBsPacketFactory::SPkt_Player_Weapon(uint32_t player_id, FBProtocol::WEAPON_TYPE weapon_type)
{
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_Player_Weapon
	/// > {
	/// > 	player_id: ulong;			// 8 bytes
	/// > 	weapon_type: WEAPON_TYPE;	// 1 byte
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

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


/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� SEND [ MONSTER ] PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��

SPtr_SendPktBuf FBsPacketFactory::SPkt_NewMonster(std::vector<MonsterSnapShot>& new_monsters)
{
	///  >  ������������������������
	/// > table Phero{
	///	> 		id				: uint;
	///	> 		offset_dist		: Position_Vec2;
	/// > }
	///  > ������������������������0
	/// > table Monster{
	/// > 		id		:  uint;
	/// > 		type	:  ubyte; // UINT8
	/// > 		pos		:  Position_Vec2;
	/// > 
	/// > 		pheros	: string ;
	/// > 
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_NewMonster
	/// > {
	/// > 	new_monsters: [Monster] ;	// ���� ũ�� // Created New Monster Infos 
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder{};

	
	std::vector<flatbuffers::Offset<FBProtocol::Monster>> MonsterSnapShots_Vector;

	/// +------------------------------------------------------------------------------------------
	///	Monster ���� ���� 
	/// ------------------------------------------------------------------------------------------+
	for (MonsterSnapShot& p : new_monsters) {
		auto pos     = FBProtocol::CreatePosition_Vec2(builder, p.Position.x, p.Position.z);
		auto pheros  = builder.CreateString(p.Pheros);
		auto Monster = FBProtocol::CreateMonster(builder, p.ID, p.Type, pos, pheros); 
		MonsterSnapShots_Vector.push_back(Monster);
	}


	auto MonsterSnapShots_Offset = builder.CreateVector(MonsterSnapShots_Vector);
	auto ServerPacket            = FBProtocol::CreateSPkt_NewMonster(builder, MonsterSnapShots_Offset);
	builder.Finish(ServerPacket);
	SPtr_SendPktBuf sendBuffer   = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_NewMonster);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_DeadMonster(uint32_t monster_id, Vec2 dead_point)
{
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_DeadMonster
	/// > {
	/// > 	id			: uint;
	/// > 	dead_point	: Position_Vec2; ( float , float )
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�'
	
	flatbuffers::FlatBufferBuilder builder{};
	auto dead_p       = FBProtocol::CreatePosition_Vec2(builder, dead_point.x, dead_point.y); // x , z ( y�ƴ� )
	auto serverPacket = FBProtocol::CreateSPkt_DeadMonster(builder, monster_id, dead_p);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_DeadMonster);
	return SPtr_SendPktBuf();
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_RemoveMonster(uint32_t monster_id)
{
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_RemoveMonster
	/// > {
	/// > 	monster_id: int;	// 4 bytes
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	
	flatbuffers::FlatBufferBuilder builder{};

	auto serverPacket = FBProtocol::CreateSPkt_RemoveMonster(builder, monster_id);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_RemoveMonster);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Monster_Transform(uint32_t monster_id, Vec3 pos, Vec3 rot)
{
	///> �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	///> table SPkt_Monster_Transform
	///> {
	///> 	monster_id: int;	// 4 bytes
	///> 	trans: Transform;				// 24 bytes (Vector3 * 2)
	///> }
	///> �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder{};

	auto position  = FBProtocol::CreateVector3(builder, pos.x, pos.y, pos.z);
	auto rotation  = FBProtocol::CreateVector3(builder, rot.x, rot.y, rot.z);
	auto transform = FBProtocol::CreateTransform(builder, position, rotation);

	auto serverPacket = FBProtocol::CreateSPkt_Monster_Transform(builder, monster_id, transform);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Monster_Transform);
	return sendBuffer;

	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Monster_HP(uint32_t monster_id, float hp)
{

	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_Monster_HP
	/// > {
	/// > 	monster_id: int;		// 4 bytes
	/// > 	hp: float;		// 4 bytes
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder{};

	auto serverPacket = FBProtocol::CreateSPkt_Monster_HP(builder, monster_id, hp);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Monster_HP);
	return sendBuffer;



	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Monster_State(uint32_t monster_id, FBProtocol::MONSTER_BT_TYPE state)
{

	///> �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	///> table SPkt_Monster_State
	///> {
	///> 	monster_id: int;			// 4 bytes
	///> 	state: MONSTER_STATE_TYPE;	// 1 byte
	///> 
	///> }
	///> �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder{};

	auto serverPacket = FBProtocol::CreateSPkt_Monster_State(builder, monster_id, state);
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


/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� SEND [ BULLET ] PACKET ��
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��

SPtr_SendPktBuf FBsPacketFactory::SPkt_Bullet_OnShoot(uint32_t player_id, FBProtocol::WEAPON_TYPE gun_id, uint32_t bullet_id, Vec3 ray)
{

	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	/// > table SPkt_Bullet_OnShoot
	/// > {
	/// > 	player_id	: int; // 4 bytes - � �÷��̾ 
	/// > 	gun_id		: int; // 4 bytes - � ���̰� 
	/// > 	bullet_id	: int; // 4 bytes - � �Ѿ��� ���°�
	/// > 
	/// > 	ray			: Vector3; // 12 bytes (4bytes * 3) - �ѱ� ������ ��Ѱ�? 
	/// > }
	/// > �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder{};

	auto Ray = FBProtocol::CreateVector3(builder, ray.x, ray.y, ray.z);

	auto serverPacket = FBProtocol::CreateSPkt_Bullet_OnShoot(builder, player_id, gun_id, bullet_id, Ray);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Bullet_OnShoot);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Bullet_OnCollision(uint32_t player_id, FBProtocol::WEAPON_TYPE gun_id, uint32_t bullet_id)
{
	///> �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�
	///> table SPkt_Bullet_OnCollision
	///> {
	///> 	player_id	: int; // 4 bytes - � �÷��̾ 
	///> 	gun_id		: int; // 4 bytes - � ���̰� 
	///> 	bullet_id	: int; // 4 bytes - � �Ѿ��� �浹�ߴ°�?
	///> }
	///> �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�

	flatbuffers::FlatBufferBuilder builder{};


	auto serverPacket = FBProtocol::CreateSPkt_Bullet_OnCollision(builder, player_id, gun_id, bullet_id);
	builder.Finish(serverPacket);
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(builder.GetBufferPointer(), static_cast<uint16_t>(builder.GetSize()), FBsProtocolID::SPkt_Bullet_OnCollision);
	return sendBuffer;
}


/// ��---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	�� UTILITY  
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------��

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


