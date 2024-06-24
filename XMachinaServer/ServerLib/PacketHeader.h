#pragma once

//#define CONNECT_WITH_TEST_CLIENT
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

	/* JUST FOR DEBUGGING NOT USE WITH TEST CLIENT PROTOCOL */
	constexpr UINT16 CPkt_NewPlayer = -1;
	constexpr UINT16 SPkt_NewPlayer = -1;

	constexpr UINT16 CPkt_RemovePlayer = -1;
	constexpr UINT16 SPkt_RemovePlayer = -1;

	constexpr UINT16 CPkt_PlayerAnimation = -1;
	constexpr UINT16 SPkt_PlayerAnimation = -1;
};
#else 
/* CONNECT WITH X-MACHINA */
namespace FBsProtocolID
{
    constexpr UINT16 CPkt_LogIn                 = 1000;
    constexpr UINT16 SPkt_LogIn                 = 1001;

    constexpr UINT16 CPkt_Chat                  = 1002;
    constexpr UINT16 SPkt_Chat                  = 1003;

    constexpr UINT16 CPkt_NetworkLatency        = 1004;
    constexpr UINT16 SPkt_NetworkLatency        = 1005;

    constexpr UINT16 CPkt_EnterGame             = 1006;
    constexpr UINT16 SPkt_EnterGame             = 1007;

    /// +-----------------------------------------------------------------------
    ///  PLAYER PACKET HEADER PROTOCOL TYPE
    /// -----------------------------------------------------------------------+
    constexpr UINT16 CPkt_NewPlayer             = 1008;
    constexpr UINT16 SPkt_NewPlayer             = 1009;

    constexpr UINT16 CPkt_RemovePlayer          = 1010;
    constexpr UINT16 SPkt_RemovePlayer          = 1011;

    constexpr UINT16 CPkt_Player_Transform      = 1012;
    constexpr UINT16 SPkt_Player_Transform      = 1013;

    constexpr UINT16 CPkt_Player_Animation      = 1014;
    constexpr UINT16 SPkt_Player_Animation      = 1015;

    constexpr UINT16 CPkt_Player_Weapon         = 1016;
    constexpr UINT16 SPkt_PlayerWeapon          = 1017;

    constexpr UINT16 CPkt_NewMonster            = 1018;
    constexpr UINT16 SPkt_NewMonster            = 1019;

    constexpr UINT16 CPkt_RemoveMonster         = 1020;
    constexpr UINT16 SPkt_RemoveMonster         = 1021;

    constexpr UINT16 CPkt_Monster_Transform     = 1022;
    constexpr UINT16 SPkt_Monster_Transform     = 1023;

    constexpr UINT16 CPkt_Monster_HP            = 1024;
    constexpr UINT16 SPkt_Monster_HP            = 1025;

    constexpr UINT16 CPkt_Monster_State         = 1026;
    constexpr UINT16 SPkt_Monster_State         = 1027;

    constexpr UINT16 CPkt_Bullet_OnShoot        = 1028;
    constexpr UINT16 SPkt_Bullet_OnShoot        = 1029;

    constexpr UINT16 CPkt_Bullet_OnCollision    = 1030;
    constexpr UINT16 SPkt_Bullet_OnCollision    = 1031;

};

#endif


struct PacketHeader
{
	UINT16 PacketSize  = 0;
	UINT16 ProtocolID  = 0;
};

