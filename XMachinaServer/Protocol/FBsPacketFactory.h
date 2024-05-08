#pragma once

/// +-----------------------------------------------
///					FBsPacketFactory 
/// ________________________________________________
/// 
/// 클라이언트로 부터 받은 패킷을 해석하는 역할을 담당한다. 
/// 
/// -----------------------------------------------+
#undef max 
#include <flatbuffers/flatbuffers.h>
#include "FBProtocol_generated.h"
#include "Contents/GamePlayer.h"

using FBSPKT_FACTORY = class FBsPacketFactory;

#define FBS_FACTORY FBsPacketFactory::GetInst()

class FBsPacketFactory
{
	DECLARE_SINGLETON(FBsPacketFactory);

public:
	static bool ProcessFBsPacket(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen);

private:
	/* ※ INVALID ※ */
	static bool Process_CPkt_Invalid(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen);
	
	/* LOG IN */
	static bool Process_CPkt_LogIn(SPtr_Session session, const FBProtocol::CPkt_LogIn& pkt);
	/* ENTER GAME */
	static bool Process_CPkt_EnterGame(SPtr_Session session, const FBProtocol::CPkt_EnterGame& pkt);
	/* NEW PLAYER */
	static bool Process_CPkt_NewPlayer(SPtr_Session session, const FBProtocol::CPkt_NewPlayer& pkt);
	/* TRANSFORM */
	static bool Process_CPkt_Transform(SPtr_Session session, const FBProtocol::CPkt_Transform& pkt);
	/* KEY INPUT */
	static bool Process_CPkt_KeyInput(SPtr_Session session, const FBProtocol::CPkt_KeyInput& pkt);
	/* PLAYER STATE */
	static bool Process_CPkt_PlayerState(SPtr_Session session, const FBProtocol::CPkt_PlayerState& pkt);
	
	/* NETWORK LATENCY */
	static bool Process_CPkt_NetworkLatency(SPtr_Session session, const FBProtocol::CPkt_NetworkLatency& pkt);
	/* CHAT */
	static bool Process_CPkt_Chat(SPtr_Session session, const FBProtocol::CPkt_Chat& pkt);

public:
	SPtr_SendPktBuf SPkt_Chat(UINT32 sessionID, std::string msg);
	SPtr_SendPktBuf SPkt_Transform(uint64_t ID, Vec3 Pos, Vec3 Rot, Vec3 Scale, Vec3 SpineLookDir, long long latency);
	SPtr_SendPktBuf SPkt_NewtorkLatency(long long timestamp);
	SPtr_SendPktBuf SPkt_LogIn(PlayerInfo& plinfo, std::vector<PlayerInfo>& remotePlayers, bool& IsSuccess);
	SPtr_SendPktBuf SPkt_NewPlayer(PlayerInfo& newPlayerInfo);
	SPtr_SendPktBuf SPkt_RemovePlayer(int removeSessionID);


private:
	static Vec3 GetVector3(const FBProtocol::Vector3* vec3);


};

