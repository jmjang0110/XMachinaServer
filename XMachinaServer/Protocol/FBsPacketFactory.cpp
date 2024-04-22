#include "pch.h"
#include "FBsPacketFactory.h"
#include "ServerLib/PacketHeader.h"

#include "../Framework.h"
#include "../ServerLib/SendBuffersFactory.h"
#include "../Contents/GameSession.h"
#include "../ServerLib/SocketData.h"



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
		const FBProtocol::CPkt_LogIn* packet = flatbuffers::GetRoot<FBProtocol::CPkt_LogIn>(DataPtr);
		if (!packet) return false;
		Process_CPkt_LogIn(session, *packet);
	}
	break;
	case FBsProtocolID::CPkt_EnterGame:
	{
		const FBProtocol::CPkt_EnterGame* packet = flatbuffers::GetRoot<FBProtocol::CPkt_EnterGame>(DataPtr);
		if (!packet) return false;
		Process_CPkt_EnterGame(session, *packet);
	}
	break;
	case FBsProtocolID::CPkt_Chat:
	{
		const FBProtocol::CPkt_Chat* packet = flatbuffers::GetRoot<FBProtocol::CPkt_Chat>(DataPtr);
		if (!packet) return false;
		Process_CPkt_Chat(session, *packet);
	}
	break;
	case FBsProtocolID::CPkt_NetworkLatency:
	{
		const FBProtocol::CPkt_NetworkLatency* packet = flatbuffers::GetRoot<FBProtocol::CPkt_NetworkLatency>(DataPtr);
		if (!packet) return false;
		Process_CPkt_NetworkLatency(session, *packet);
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

	std::cout << "Now : "		<< end.time_since_epoch().count() / 1e-6 <<
				" pkt_time : "	<< pkt_time.time_since_epoch().count() / 1e-6
		<< "Server Latency: " << std::chrono::duration_cast<std::chrono::milliseconds>(latency).count() << "ms" << std::endl;

	session->Send(FRAMEWORK->GetSendFactory()->SPkt_NewtorkLatency(timestamp)); // 클라이언트에 받았던 시간 그대로를 서버 패킷으로 만들어서 다시 클라에게 보낸다. 

	return true;
}

bool FBsPacketFactory::Process_CPkt_LogIn(SPtr_Session session, const FBProtocol::CPkt_LogIn& pkt)
{
	//SPtr_GameSession gameSession = std::static_pointer_cast<GameSession>(session);
	//std::cout << "LOG IN SESSION ID : " << gameSession->GetID();
	//std::wcout << L" IP : " << gameSession->GetSocketData().GetIpAddress().c_str() << std::endl;



	return true;
}

bool FBsPacketFactory::Process_CPkt_EnterGame(SPtr_Session session, const FBProtocol::CPkt_EnterGame& pkt)
{
	

	return true;
}
