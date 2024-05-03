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
using FBSPKT_FACTORY = class FBsPacketFactory;
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


};

