#pragma once

namespace FBsProtocolID
{
	constexpr UINT16 CPkt_LogIn     = 1000;
	constexpr UINT16 SPkt_LogIn     = 1001;

	constexpr UINT16 CPkt_EnterGame = 1002;
	constexpr UINT16 SPkt_EnterGame = 1003;

	constexpr UINT16 CPkt_Chat      = 1004;
	constexpr UINT16 SPkt_Chat      = 1005;

	constexpr UINT16 CPkt_Transform = 1006;
	constexpr UINT16 SPkt_Transform = 1007;

};

struct PacketHeader
{
	UINT16 PacketSize  = 0;
	UINT16 ProtocolID  = 0;
};

