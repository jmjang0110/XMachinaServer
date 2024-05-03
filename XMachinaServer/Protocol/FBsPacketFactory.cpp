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
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_NetworkLatency ]\n");

		const FBProtocol::CPkt_NetworkLatency* packet = flatbuffers::GetRoot<FBProtocol::CPkt_NetworkLatency>(DataPtr);
		if (!packet) return false;
		Process_CPkt_NetworkLatency(session, *packet);
	}
	break;
	case FBsProtocolID::CPkt_Transform:
	{
		LOG_MGR->Cout(session->GetID(), " - RECV - ", "[ CPkt_Transform ]\n");
		
		const FBProtocol::CPkt_Transform* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Transform>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Transform(session, *packet);
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

bool FBsPacketFactory::Process_CPkt_NetworkLatency(SPtr_Session session, const FBProtocol::CPkt_NetworkLatency& pkt)
{
	// 패킷으로부터 long long으로 시간을 받음
	long long timestamp = pkt.timestamp();
	auto end            = std::chrono::steady_clock::now();
	auto pkt_time       = std::chrono::time_point<std::chrono::steady_clock>(std::chrono::milliseconds(timestamp));
	auto latency        = end - pkt_time;

	/*std::cout << "Now : "		<< end.time_since_epoch().count() / 1e-6 <<
				" pkt_time : "	<< pkt_time.time_since_epoch().count() / 1e-6
		<< "Server Latency: " << std::chrono::duration_cast<std::chrono::milliseconds>(latency).count() << "ms" << std::endl;*/


	return true;
}

bool FBsPacketFactory::Process_CPkt_LogIn(SPtr_Session session, const FBProtocol::CPkt_LogIn& pkt)
{
#ifdef CONNECT_WITH_TEST_CLIENT
	return true;
#endif

	SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);

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
	auto SendPkt_LogIn     = SEND_FACTORY->SPkt_LogIn(MyInfo, RemotePlayersInfo, LogInSuccess);
	auto SendPkt_NewPlayer = SEND_FACTORY->SPkt_NewPlayer(MyInfo);

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
