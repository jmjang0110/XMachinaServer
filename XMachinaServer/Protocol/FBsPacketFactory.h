#pragma once

/// +-----------------------------------------------
///					FBsPacketFactory 
/// ________________________________________________
/// 
/// Ŭ���̾�Ʈ�� ���� ���� ��Ŷ�� �ؼ��ϴ� ������ ����Ѵ�. 
/// 
/// -----------------------------------------------+
#undef max 
#include <flatbuffers/flatbuffers.h>
#include "FBProtocol_generated.h"
#include "Enum_generated.h"
#include "Struct_generated.h"
#include "Transform_generated.h"

#include "Contents/GamePlayer.h"
#include "Contents/GameMonster.h"


using FBSPKT_FACTORY = class FBsPacketFactory;

#define FBS_FACTORY FBsPacketFactory::GetInst()

namespace PLAYER_MOVE_STATE {
	constexpr int32_t Start    = 0;
	constexpr int32_t Progress = 1;
	constexpr int32_t End      = 2;

}

class FBsPacketFactory
{
	DECLARE_SINGLETON(FBsPacketFactory);

public:
	static bool ProcessFBsPacket(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen);

private:
	/* INVALID, LOGIN, ENTER_GAME, LATENCY, CHAT */
	static bool Process_CPkt_Invalid(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen);
	static bool Process_CPkt_LogIn(SPtr_Session session, const FBProtocol::CPkt_LogIn& pkt);
	static bool Process_CPkt_EnterGame(SPtr_Session session, const FBProtocol::CPkt_EnterGame& pkt);
	static bool Process_CPkt_NetworkLatency(SPtr_Session session, const FBProtocol::CPkt_NetworkLatency& pkt);
	static bool Process_CPkt_Chat(SPtr_Session session, const FBProtocol::CPkt_Chat& pkt);

	/* PLAYER */
	static bool Process_CPkt_NewPlayer(SPtr_Session session, const FBProtocol::CPkt_NewPlayer& pkt);
	static bool Process_CPkt_RemovePlayer(SPtr_Session session, const FBProtocol::CPkt_RemovePlayer& pkt);
	static bool Process_CPkt_Player_Transform(SPtr_Session session, const FBProtocol::CPkt_Player_Transform& pkt);
	static bool Process_CPkt_Player_Animation(SPtr_Session session, const FBProtocol::CPkt_Player_Animation& pkt);
	static bool Process_CPkt_Player_Weapon(SPtr_Session session, const FBProtocol::CPkt_Player_Weapon& pkt);

	/* MONSTER */
	static bool Process_CPkt_NewMonster(SPtr_Session session, const FBProtocol::CPkt_NewMonster& pkt);
	static bool Process_CPkt_RemoveMonster(SPtr_Session session, const FBProtocol::CPkt_RemoveMonster& pkt);
	static bool Process_CPkt_Monster_Transform(SPtr_Session session, const FBProtocol::CPkt_Monster_Transform& pkt);
	static bool Process_CPkt_Monster_HP(SPtr_Session session, const FBProtocol::CPkt_Monster_HP& pkt);
	static bool Process_CPkt_Monster_State(SPtr_Session session, const FBProtocol::CPkt_Monster_State& pkt);

	/* BULLET */
	static bool Process_CPkt_Bullet_OnShoot(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnShoot& pkt);
	static bool Process_CPkt_Bullet_OnCollision(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnCollision& pkt);

public:
	/* LOGIN, LATENCY, CHAT */
	SPtr_SendPktBuf SPkt_LogIn(bool success);
	SPtr_SendPktBuf SPkt_EnterGame(PlayerInfo& myinfo, std::vector<PlayerInfo>& players);
	SPtr_SendPktBuf SPkt_NetworkLatency(long long timestamp);
	SPtr_SendPktBuf SPkt_Chat(uint32_t player_id, std::string msg);

	/* PLAYER */
	SPtr_SendPktBuf SPkt_NewPlayer(PlayerInfo& newplayer);
	SPtr_SendPktBuf SPkt_RemovePlayer(uint32_t player_id);
	SPtr_SendPktBuf SPkt_Player_Transform(uint32_t player_id, int32_t move_state, long long latency, float velocity, Vec3 movedir, Vec3 pos, Vec3 rot, Vec3 spine_look, float animparam_h, float animparam_v);
	SPtr_SendPktBuf SPkt_Player_Animation(uint32_t player_id, int anim_upper_idx, int anim_lower_idx, float anim_param_h, float anim_param_v);
	SPtr_SendPktBuf SPkt_Player_Weapon(uint32_t player_id, FBProtocol::WEAPON_TYPE weapon_type);

	/* MONSTER */
	SPtr_SendPktBuf SPkt_NewMonster(std::vector<MonsterInfo>& new_monsters);
	SPtr_SendPktBuf SPkt_RemoveMonster(uint32_t monster_id);
	SPtr_SendPktBuf SPkt_Monster_Transform(uint32_t monster_id, FBProtocol::Transform trans);
	SPtr_SendPktBuf SPkt_Monster_HP(uint32_t monster_id, float hp);
	SPtr_SendPktBuf SPkt_Monster_State(uint32_t monster_id, FBProtocol::MONSTER_STATE_TYPE state);

	/* BULLET */
	SPtr_SendPktBuf SPkt_Bullet_OnShoot(uint32_t player_id, uint32_t gun_id, uint32_t bullet_id, Vec3 ray);
	SPtr_SendPktBuf SPkt_Bullet_OnCollision(uint32_t player_id, uint32_t gun_id, uint32_t bullet_id);

private:
	static Vec3 GetVector3(const FBProtocol::Vector3* vec3);
	static Vec4 GetVector4(const FBProtocol::Vector4* vec4);


};

