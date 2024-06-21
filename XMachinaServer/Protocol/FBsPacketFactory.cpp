#include "pch.h"
#include "FBsPacketFactory.h"
#include "ServerLib/PacketHeader.h"

/* RELATED FLAT BUFFERS HEADER */
#include "Enum_generated.h"
#include "FBProtocol_generated.h"
#include "Struct_generated.h"
#include "Transform_generated.h"

#include "../Framework.h"
#include "../ServerLib/SendBuffersFactory.h"
#include "../Contents/GameSession.h"
#include "../ServerLib/SocketData.h"
#include "Contents/GamePlayer.h"
#include "Contents/GameManager.h"


DEFINE_SINGLETON(FBsPacketFactory);

/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ PROCESS CLIENT PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★
bool FBsPacketFactory::ProcessFBsPacket(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen)
{

	/* ▶ Packet Struct ------------------------------------------------- */
	/* [PacketHeader(ProtocolID, PktSize)][DATA-(FlatBuffers Serialized)] */
	/* ------------------------------------------------------------------ */

	PacketHeader* Head  = reinterpret_cast<PacketHeader*>(packetBuf);
	const void* DataPtr = packetBuf + sizeof(PacketHeader);

	switch (Head->ProtocolID)
	{	
	case FBsProtocolID::CPkt_LogIn:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_LogIn ]\n");
		const FBProtocol::CPkt_LogIn* packet = flatbuffers::GetRoot<FBProtocol::CPkt_LogIn>(DataPtr);
		if (!packet) return false;
		Process_CPkt_LogIn(session, *packet);
	}
	break;
	case FBsProtocolID::CPkt_EnterGame:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_EnterGame ]\n");

		const FBProtocol::CPkt_EnterGame* packet = flatbuffers::GetRoot<FBProtocol::CPkt_EnterGame>(DataPtr);
		if (!packet) return false;
		Process_CPkt_EnterGame(session, *packet);
	}
	break;
	case FBsProtocolID::CPkt_Chat:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_Chat ]\n");
		const FBProtocol::CPkt_Chat* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Chat>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Chat(session, *packet);
	}
	break;
	case FBsProtocolID::CPkt_NetworkLatency:
	{
		//LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_NetworkLatency ]\n");

		const FBProtocol::CPkt_NetworkLatency* packet = flatbuffers::GetRoot<FBProtocol::CPkt_NetworkLatency>(DataPtr);
		if (!packet) return false;
		Process_CPkt_NetworkLatency(session, *packet);
	}
	break;
	case FBsProtocolID::CPkt_Transform:
	{
		//LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_Transform ]\n");
		
		const FBProtocol::CPkt_Transform* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Transform>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Transform(session, *packet);
	}
	break;
	case FBsProtocolID::CPkt_PlayerAnimation:
	{

		const FBProtocol::CPkt_PlayerAnimation* packet = flatbuffers::GetRoot<FBProtocol::CPkt_PlayerAnimation>(DataPtr);
		if (!packet) return false;
		Process_CPkt_PlayerAnimation(session, *packet);
	}
	break;
	}

	return true;
}

bool FBsPacketFactory::Process_CPkt_Invalid(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen)
{
	return false;
}

bool FBsPacketFactory::Process_CPkt_Chat(SPtr_Session session, const FBProtocol::CPkt_Chat& pkt)
{
	std::cout << "CPKT CHAT [" << session->GetID() << "] - SESSION : " << session.get() << " DATA : " <<
		pkt.message()->c_str() << std::endl;
	return true;
}

bool FBsPacketFactory::Process_CPkt_PlayerAnimation(SPtr_Session session, const FBProtocol::CPkt_PlayerAnimation& pkt)
{
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

	int ObjectID                = session->GetID();
	int32_t animation_upper_idx = pkt.animation_upper_index();
	int32_t animation_lower_idx = pkt.animation_lower_index();
	int32_t animation_param_h   = pkt.animation_param_h();
	int32_t animation_param_v   = pkt.animation_param_v();
	

	/* 클라이언트의 패킷을 그대로 다시 보낸다. */
	auto spkt = FBS_FACTORY->SPkt_PlayerAnimation(ObjectID, animation_upper_idx, animation_lower_idx, animation_param_h, animation_param_v);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayerInfo().RoomID, spkt, gameSession->GetID());

	return true;
}

bool FBsPacketFactory::Process_CPkt_NetworkLatency(SPtr_Session session, const FBProtocol::CPkt_NetworkLatency& pkt)
{
	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);


	// 패킷으로부터 long long으로 시간을 받음
	long long timestamp = pkt.timestamp();
	
	/* 클라이언트의 패킷을 그대로 다시 보낸다. */
	auto spkt = FBS_FACTORY->SPkt_NewtorkLatency(timestamp);

	//session->Send(spkt);
	GAME_MGR->Send(spkt, gameSession->GetPlayerInfo().RoomID, session->GetID());

	return true;
}

bool FBsPacketFactory::Process_CPkt_LogIn(SPtr_Session session, const FBProtocol::CPkt_LogIn& pkt)
{
#ifdef CONNECT_WITH_TEST_CLIENT
	return true;
#endif

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	
	gameSession->GetPlayer()->SetPosition(Vec3(25, 0, 260));

	LOG_MGR->SetColor(TextColor::BrightBlue);
	LOG_MGR->Cout("LOG IN SESSION ID : ", gameSession->GetID());
	LOG_MGR->WCout(L"-- LOG-IN-IP : IPv4-", gameSession->GetSocketData().GetIpAddress().c_str(), '\n');
	LOG_MGR->SetColor(TextColor::Default);

	/// +---------------------------------------------------------------------------------------------------------------
	PlayerInfo				MyInfo            = gameSession->GetPlayerInfo();  // MY GAME PLAYER INFO 
	bool					LogInSuccess      = true;						   // IS MY GAME PLAYER SUCCESS ?
	std::vector<PlayerInfo> RemotePlayersInfo = GAME_MGR->GetPlayerInfos_Room(gameSession->GetPlayerInfo().RoomID); // REMOTE PLAYERS INFO IN ROOM ( MY PLAYER ROOM ID )
	/// ---------------------------------------------------------------------------------------------------------------+

	/// +-----------------------------------
	/// GET SEND PKT ( LOG IN / NEW PLAYER )
	/// -----------------------------------+
	auto SendPkt_LogIn     = FBS_FACTORY->SPkt_LogIn(MyInfo, RemotePlayersInfo, LogInSuccess);
	auto SendPkt_NewPlayer = FBS_FACTORY->SPkt_NewPlayer(MyInfo);

	/// +--------------------------------
	/// SEND LOG IN PKT TO ME ( SESSION )
	/// --------------------------------+
	session->Send(SendPkt_LogIn);

	/// +---------------------------------------------------------------------------------------
	/// SEND NEW PLAYER PKT TO SESSIONS IN ROOM ( SESSION->GET ROOM ID ) - EXCEPT ME ( SESSION )
	/// ---------------------------------------------------------------------------------------+
	GAME_MGR->BroadcastRoom(gameSession->GetPlayerInfo().RoomID, SendPkt_NewPlayer, gameSession->GetID());



	return true;
}

bool FBsPacketFactory::Process_CPkt_EnterGame(SPtr_Session session, const FBProtocol::CPkt_EnterGame& pkt)
{
	

	return true;
}

bool FBsPacketFactory::Process_CPkt_NewPlayer(SPtr_Session session, const FBProtocol::CPkt_NewPlayer& pkt)
{
	return true;
}

bool FBsPacketFactory::Process_CPkt_Transform(SPtr_Session session, const FBProtocol::CPkt_Transform& pkt)
{
	SPtr_GameSession gameSession		= std::static_pointer_cast<GameSession>(session);
	UINT32 id							= session->GetID();

	long long				latency     = pkt.latency(); /* 해당 클라이언트의 평균 Latency (ms)  */
	float					Vel         = pkt.velocity();
	Vec3					MoveDir		= GetVector3(pkt.movedir());
	Vec3					pos         = GetVector3(pkt.trans()->position());
	Vec3					rot         = GetVector3(pkt.trans()->rotation());
	int32_t					movestate   = pkt.move_state();
	Vec3					SDir        = GetVector3(pkt.spine_look());

	float					animparam_h = pkt.animparam_h();
	float					animparam_v = pkt.animparam_v();

	gameSession->GetPlayer()->SetPosition(pos);
	SPtr_SendPktBuf SendPkt = FBS_FACTORY->SPkt_Transform(id, pos, rot, movestate, MoveDir, Vel, SDir, latency, animparam_h, animparam_v);
	GAME_MGR->BroadcastRoom(gameSession->GetPlayerInfo().RoomID, SendPkt, id);

	return true;
}

bool FBsPacketFactory::Process_CPkt_KeyInput(SPtr_Session session, const FBProtocol::CPkt_KeyInput& pkt)
{
	return true;
}

bool FBsPacketFactory::Process_CPkt_PlayerState(SPtr_Session session, const FBProtocol::CPkt_PlayerState& pkt)
{
	return true;
}

/// ★---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
///	◈ SEND SERVER PACKET ◈
/// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------★
SPtr_SendPktBuf FBsPacketFactory::SPkt_Chat(UINT32 sessionID, std::string msg)
{
	flatbuffers::FlatBufferBuilder builder;

	auto msgOffset = builder.CreateString(msg);
	auto ServerPacket = FBProtocol::CreateSPkt_Chat(builder, sessionID, msgOffset);

	builder.Finish(ServerPacket);

	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBsProtocolID::SPkt_Chat);
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_Transform(uint64_t ID, Vec3 Pos, Vec3 Rot, int32_t movestate, Vec3 MoveDir, float velocity, Vec3 SpineLookDir, long long latency
, float animparam_h, float animparam_v)
{
	flatbuffers::FlatBufferBuilder builder{};

	auto MoveDirection = FBProtocol::CreateVector3(builder, MoveDir.x, MoveDir.y, MoveDir.z);
	auto position      = FBProtocol::CreateVector3(builder, Pos.x, Pos.y, Pos.z);
	auto rotation      = FBProtocol::CreateVector3(builder, Rot.x, Rot.y, Rot.z);
	auto transform     = FBProtocol::CreateTransform(builder, position, rotation);
	auto Spine_LookDir = FBProtocol::CreateVector3(builder, SpineLookDir.x, SpineLookDir.y, SpineLookDir.z);

	auto ServerPacket = FBProtocol::CreateSPkt_Transform(builder, ID, latency, movestate, velocity, MoveDirection, transform, Spine_LookDir, animparam_h, animparam_v);
	builder.Finish(ServerPacket);


	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_Transform);
	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_NewtorkLatency(long long timestamp)
{
	flatbuffers::FlatBufferBuilder builder;

	auto ServerPacket = FBProtocol::CreateSPkt_NetworkLatency(builder, timestamp);

	builder.Finish(ServerPacket);

	const uint8_t* bufferPtr = builder.GetBufferPointer();
	const uint16_t serializedDataSize = static_cast<uint16_t>(builder.GetSize());

	return SEND_FACTORY->CreatePacket(bufferPtr, serializedDataSize, FBsProtocolID::SPkt_NetworkLatency);
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_LogIn(PlayerInfo& plinfo, std::vector<PlayerInfo>& remotePlayers, bool& IsSuccess)
{
	flatbuffers::FlatBufferBuilder builder;

	/* Create ServerPacket */
	bool successOffset = true;
	std::vector<flatbuffers::Offset<FBProtocol::Player>> PlayerInfos_vector;

	/* My Player Info */
	auto position  = FBProtocol::CreateVector3(builder, plinfo.Position.x, plinfo.Position.y, plinfo.Position.z);
	auto rotation  = FBProtocol::CreateVector3(builder, plinfo.Rotation.x, plinfo.Rotation.y, plinfo.Rotation.z);
	auto transform = FBProtocol::CreateTransform(builder, position, rotation);

	auto Spine_LookDir = FBProtocol::CreateVector3(builder, plinfo.SpineDir.x, plinfo.SpineDir.y, plinfo.SpineDir.z);
	auto Myinfo = CreatePlayer(builder, plinfo.PlayerID, builder.CreateString(plinfo.Name), plinfo.Type, transform, Spine_LookDir);

	/* Remote Players */
	for (PlayerInfo& p : remotePlayers) {
		auto ID = p.PlayerID;
		auto name = builder.CreateString(p.Name);
		auto PlayerInfoType = p.Type;

		auto position  = FBProtocol::CreateVector3(builder, p.Position.x, p.Position.y, p.Position.z);
		auto rotation  = FBProtocol::CreateVector3(builder, p.Rotation.x, p.Rotation.y, p.Rotation.z);
		auto transform = FBProtocol::CreateTransform(builder, position, rotation);

		auto Spine_LookDir = FBProtocol::CreateVector3(builder, p.SpineDir.x, p.SpineDir.y, p.SpineDir.z);


		auto PlayerInfo = CreatePlayer(builder, ID, name, PlayerInfoType, transform, Spine_LookDir); // CreatePlayerInfo는 스키마에 정의된 함수입니다.
		PlayerInfos_vector.push_back(PlayerInfo);
	}
	auto PlayerInfosOffset = builder.CreateVector(PlayerInfos_vector);



	/* CREATE LOG IN PACKET */
	auto ServerPacket = FBProtocol::CreateSPkt_LogIn(builder, successOffset, Myinfo, PlayerInfosOffset);
	builder.Finish(ServerPacket);

	/* Create SendBuffer */
	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;

	return SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_LogIn);
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_NewPlayer(PlayerInfo& newPlayerInfo)
{
	flatbuffers::FlatBufferBuilder builder{};

	auto ID = newPlayerInfo.PlayerID;
	auto name = builder.CreateString(newPlayerInfo.Name);
	auto PlayerInfoType = newPlayerInfo.Type;

	auto position      = FBProtocol::CreateVector3(builder, newPlayerInfo.Position.x, newPlayerInfo.Position.y, newPlayerInfo.Position.z);
	auto rotation      = FBProtocol::CreateVector3(builder, newPlayerInfo.Rotation.x, newPlayerInfo.Rotation.y, newPlayerInfo.Rotation.z);
	auto transform     = FBProtocol::CreateTransform(builder, position, rotation);
	auto Spine_LookDir = FBProtocol::CreateVector3(builder, newPlayerInfo.SpineDir.x, newPlayerInfo.SpineDir.y, newPlayerInfo.SpineDir.z);
	auto PlayerInfo    = CreatePlayer(builder, ID, name, PlayerInfoType, transform, Spine_LookDir); // CreatePlayerInfo는 스키마에 정의된 함수입니다.

	auto ServerPacket = FBProtocol::CreateSPkt_NewPlayer(builder, PlayerInfo);
	builder.Finish(ServerPacket);

	const uint8_t* bufferPointer      = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer        = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_NewPlayer);


	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_RemovePlayer(int removeSessionID)
{
	flatbuffers::FlatBufferBuilder builder{};

	int32_t id = static_cast<int32_t>(removeSessionID);
	auto ServerPacket = FBProtocol::CreateSPkt_RemovePlayer(builder, id);
	builder.Finish(ServerPacket);

	const uint8_t* bufferPointer = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_RemovePlayer);

	return sendBuffer;
}

SPtr_SendPktBuf FBsPacketFactory::SPkt_PlayerAnimation(int ObjectID, int anim_upper_idx, int anim_lower_idx, float anim_param_h, float anim_param_v)
{
	flatbuffers::FlatBufferBuilder builder{};

	int32_t id                    = static_cast<int32_t>(ObjectID);
	int32_t animation_upper_index = static_cast<int32_t>(anim_upper_idx);
	int32_t animation_lower_index = static_cast<int32_t>(anim_lower_idx);
	float	animation_param_h     = static_cast<float>(anim_param_h);
	float	animation_param_v     = static_cast<float>(anim_param_v);


	auto ServerPacket = FBProtocol::CreateSPkt_PlayerAnimation(builder, id, animation_upper_index, animation_lower_index, animation_param_h, animation_param_v);
	builder.Finish(ServerPacket);

	const uint8_t* bufferPointer      = builder.GetBufferPointer();
	const uint16_t SerializeddataSize = static_cast<uint16_t>(builder.GetSize());;
	SPtr_SendPktBuf sendBuffer        = SEND_FACTORY->CreatePacket(bufferPointer, SerializeddataSize, FBsProtocolID::SPkt_PlayerAnimation);

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


