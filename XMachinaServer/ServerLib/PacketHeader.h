#pragma once

#define CONNECT_WITH_TEST_CLIENT
#ifdef CONNECT_WITH_TEST_CLIENT

namespace FBsProtocolID
{
	constexpr UINT16 CPkt_LogIn          = 1000;
	constexpr UINT16 SPkt_LogIn          = 1001;

	constexpr UINT16 CPkt_EnterGame      = 1002;
	constexpr UINT16 SPkt_EnterGame      = 1003;

	constexpr UINT16 CPkt_Chat           = 1004;
	constexpr UINT16 SPkt_Chat           = 1005;

	constexpr UINT16 CPkt_Transform      = 1006;
	constexpr UINT16 SPkt_Transform      = 1007;

	constexpr UINT16 CPkt_NetworkLatency = 1008;
	constexpr UINT16 SPkt_NetworkLatency = 1009;


};
#else 

namespace FBsProtocolID
{
	constexpr UINT16 CPkt_LogIn = 1000;
	constexpr UINT16 SPkt_LogIn = 1001;

	constexpr UINT16 CPkt_NewPlayer = 1002;
	constexpr UINT16 SPkt_NewPlayer = 1003;

	constexpr UINT16 CPkt_EnterGame = 1004;
	constexpr UINT16 SPkt_EnterGame = 1005;

	constexpr UINT16 CPkt_Chat = 1006;
	constexpr UINT16 SPkt_Chat = 1007;

	constexpr UINT16 CPkt_Transform = 1008;
	constexpr UINT16 SPkt_Transform = 1009;

	constexpr UINT16 CPkt_KeyInput = 1010;
	constexpr UINT16 SPkt_KeyInput = 1011;

	constexpr UINT16 CPkt_NetworkLatency = 1012;
	constexpr UINT16 SPkt_NetworkLatency = 1013;


};
#endif


struct PacketHeader
{
	UINT16 PacketSize  = 0;
	UINT16 ProtocolID  = 0;
};

