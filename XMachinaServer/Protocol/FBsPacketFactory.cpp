#include "pch.h"
#include "FBsPacketFactory.h"
#include "ServerLib/PacketHeader.h"

#include "../Framework.h"
#include "../ServerLib/SendBuffersFactory.h"

bool FBsPacketFactory::ProcessFBsPacket(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen)
{

	/* �� Packet Struct ------------------------------------------------- */
	/* [PacketHeader(ProtocolID, PktSize)][DATA-(FlatBuffers Serialized)] */
	/* ------------------------------------------------------------------ */

	PacketHeader* Head  = reinterpret_cast<PacketHeader*>(packetBuf);
	const void* DataPtr = packetBuf + sizeof(PacketHeader);
	switch (Head->ProtocolID)
	{	
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

bool FBsPacketFactory::Process_CPkt_Chat(SPtr_Session session, const FBProtocol::CPkt_Chat& pkt)
{
	std::cout << "CPKT CHAT [" << session->GetID() << "] - SESSION : " << session.get() << " DATA : " <<
		pkt.message()->c_str() << std::endl;
	return true;
}

bool FBsPacketFactory::Process_CPkt_NetworkLatency(SPtr_Session session, const FBProtocol::CPkt_NetworkLatency& pkt)
{
	// ��Ŷ���κ��� long long���� �ð��� ����
	long long timestamp = pkt.timestamp();
	auto end            = std::chrono::steady_clock::now();
	auto pkt_time       = std::chrono::time_point<std::chrono::steady_clock>(std::chrono::milliseconds(timestamp));
	auto latency        = end - pkt_time;

	std::cout << "Latency: " << std::chrono::duration_cast<std::chrono::milliseconds>(latency).count() << "ms" << std::endl;

	session->Send(FRAMEWORK->GetSendFactory()->SPkt_NewtorkLatency(timestamp)); // Ŭ���̾�Ʈ�� �޾Ҵ� �ð� �״�θ� ���� ��Ŷ���� ���� �ٽ� Ŭ�󿡰� ������. 

	return true;
}
