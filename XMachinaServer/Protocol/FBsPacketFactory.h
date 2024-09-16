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
#include "Enum_generated.h"
#include "Struct_generated.h"
#include "Transform_generated.h"

using FBSPKT_FACTORY = class FBsPacketFactory;

#define FBS_FACTORY FBsPacketFactory::GetInst()

class GameObject;
class FBsPacketFactory
{
	DECLARE_SINGLETON(FBsPacketFactory);

public:
	static bool ProcessFBsPacket(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen);

private:
	/* INVALID, LOGIN, ENTER_GAME, LATENCY, CHAT */
	static bool Process_CPkt_Invalid(SPtr_Session session, BYTE* packetBuf, UINT32 Datalen);
	static bool Process_CPkt_LogIn(SPtr_Session session, const FBProtocol::CPkt_LogIn& pkt);
	static bool Process_CPkt_EnterLobby(SPtr_Session session, const FBProtocol::CPkt_EnterLobby& pkt);
	static bool Process_CPkt_EnterGame(SPtr_Session session, const FBProtocol::CPkt_EnterGame& pkt);
	static bool Process_CPkt_PlayGame(SPtr_Session session, const FBProtocol::CPkt_PlayGame& pkt);
	static bool Process_CPkt_NetworkLatency(SPtr_Session session, const FBProtocol::CPkt_NetworkLatency& pkt);
	static bool Process_CPkt_Chat(SPtr_Session session, const FBProtocol::CPkt_Chat& pkt);

	/* PLAYER */
	static bool Process_CPkt_NewPlayer(SPtr_Session session, const FBProtocol::CPkt_NewPlayer& pkt);
	static bool Process_CPkt_RemovePlayer(SPtr_Session session, const FBProtocol::CPkt_RemovePlayer& pkt);
	static bool Process_CPkt_PlayerOnSkill(SPtr_Session session, const FBProtocol::CPkt_PlayerOnSkill& pkt);
	static bool Process_CPkt_Player_Transform(SPtr_Session session, const FBProtocol::CPkt_Player_Transform& pkt);
	static bool Process_CPkt_Player_Animation(SPtr_Session session, const FBProtocol::CPkt_Player_Animation& pkt);
	static bool Process_CPkt_Player_Weapon(SPtr_Session session, const FBProtocol::CPkt_Player_Weapon& pkt);
	static bool Process_CPkt_Player_AimRotation(SPtr_Session session, const FBProtocol::CPkt_Player_AimRotation& pkt);
	static bool Process_CPkt_Player_State(SPtr_Session session, const FBProtocol::CPkt_Player_State& pkt);

	/* MONSTER */
	static bool Process_CPkt_NewMonster(SPtr_Session session, const FBProtocol::CPkt_NewMonster& pkt);
	static bool Process_CPkt_DeadMonster(SPtr_Session session, const FBProtocol::CPkt_DeadMonster& pkt);
	static bool Process_CPkt_RemoveMonster(SPtr_Session session, const FBProtocol::CPkt_RemoveMonster& pkt);
	static bool Process_CPkt_Monster_Transform(SPtr_Session session, const FBProtocol::CPkt_Monster_Transform& pkt);
	static bool Process_CPkt_Monster_HP(SPtr_Session session, const FBProtocol::CPkt_Monster_HP& pkt);
	static bool Process_CPkt_Monster_State(SPtr_Session session, const FBProtocol::CPkt_Monster_State& pkt);

	/* PHERO */
	static bool Process_CPkt_GetPhero(SPtr_Session session, const FBProtocol::CPkt_GetPhero& pkt);

	/* BULLET */
	static bool Process_CPkt_Bullet_OnShoot(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnShoot& pkt);
	static bool Process_CPkt_Bullet_OnHitEnemy(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnHitEnemy& pkt);
	static bool Process_CPkt_Bullet_OnHitExpEnemy(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnHitExpEnemy& pkt);
	static bool Process_CPkt_Bullet_OnCollision(SPtr_Session session, const FBProtocol::CPkt_Bullet_OnCollision& pkt);

	/* ITEM */
	static bool Process_CPkt_Item_Interact(SPtr_Session session, const FBProtocol::CPkt_Item_Interact& pkt);
	static bool Process_CPkt_Item_ThrowAway(SPtr_Session session, const FBProtocol::CPkt_Item_ThrowAway& pkt);

public:
	/* LOGIN, LATENCY, CHAT */
	SPtr_SendPktBuf SPkt_LogIn(std::string name, bool success);
	SPtr_SendPktBuf SPkt_EnterGame(SPtr<GameObject>& myinfo, std::vector<SPtr<GameObject>>& players);
	SPtr_SendPktBuf SPkt_EnterLobby(int player_EnterOrder, SPtr<GameObject>& myinfo, std::vector<SPtr<GameObject>>& players);
	SPtr_SendPktBuf SPkt_PlayGame();
	SPtr_SendPktBuf SPkt_NetworkLatency(long long timestamp);
	SPtr_SendPktBuf SPkt_Chat(uint32_t player_id, std::string msg);

	/* PLAYER */
	SPtr_SendPktBuf SPkt_NewPlayer(SPtr<GameObject>& newplayer);
	SPtr_SendPktBuf SPkt_RemovePlayer(uint32_t player_id);
	SPtr_SendPktBuf SPkt_PlayerOnSkill(uint32_t player_id, FBProtocol::PLAYER_SKILL_TYPE skill_type, float phero_amount, int mindcontrol_monster_id);
	SPtr_SendPktBuf SPkt_Player_Transform(uint32_t player_id, int32_t move_state, long long latency, float velocity, Vec3 movedir, Vec3 pos, Vec3 rot, Vec3 spine_look, float animparam_h, float animparam_v);
	SPtr_SendPktBuf SPkt_Player_Animation(uint32_t player_id, int anim_upper_idx, int anim_lower_idx, float anim_param_h, float anim_param_v);
	SPtr_SendPktBuf SPkt_Player_Weapon(uint32_t player_id, FBProtocol::ITEM_TYPE weapon_type);
	SPtr_SendPktBuf SPkt_Player_AimRotation(uint32_t player_id, float aim_rotation, float spine_angle);
	SPtr_SendPktBuf SPKt_Player_State(uint32_t player_id, float hp, float phero, FBProtocol::PLAYER_STATE_TYPE state);

	/* MONSTER */
	SPtr_SendPktBuf SPkt_NewMonster(std::vector<SPtr<GameObject>>& new_monsters);
	SPtr_SendPktBuf SPkt_DeadMonster(uint32_t monster_id, Vec3 dead_point, std::string pheros);
	SPtr_SendPktBuf SPkt_RemoveMonster(uint32_t monster_id);
	SPtr_SendPktBuf SPkt_Monster_Transform(uint32_t monster_id, Vec3 pos, Vec3 look);
	SPtr_SendPktBuf SPkt_Monster_HP(uint32_t monster_id, float hp);
	SPtr_SendPktBuf SPkt_Monster_State(uint32_t monster_id, FBProtocol::MONSTER_BT_TYPE montser_bt_type, int32_t step = 0);
	SPtr_SendPktBuf SPkt_Monster_Target(uint32_t monster_id, int32_t target_player_id, int32_t target_monster_id);

	/* PHERO */
	SPtr_SendPktBuf SPkt_GetPhero(uint32_t phero_id, uint32_t player_id);

	/* BULLET */
	SPtr_SendPktBuf SPkt_Bullet_OnShoot(uint32_t player_id, FBProtocol::ITEM_TYPE  gun_id, uint32_t bullet_id, Vec3 fire_pos, Vec3 ray);
	SPtr_SendPktBuf SPkt_Bullet_OnHitEnemy(uint32_t player_id, FBProtocol::ITEM_TYPE  gun_id, uint32_t bullet_id, Vec3 ray);
	SPtr_SendPktBuf SPkt_Bullet_OnCollision(uint32_t player_id, FBProtocol::ITEM_TYPE gun_id, uint32_t bullet_id);

	/* ITEM */
	SPtr_SendPktBuf SPkt_Item_Interact(uint32_t player_id, uint32_t item_id, FBProtocol::ITEM_TYPE item_type, Vec3 drop_pos);
	SPtr_SendPktBuf SPkt_Item_ThrowAway(uint32_t player_id, uint32_t item_id, FBProtocol::ITEM_TYPE item_type, Vec3 drop_pos);

private:
	static Vec3 GetVector3(const FBProtocol::Vector3* vec3);
	static Vec4 GetVector4(const FBProtocol::Vector4* vec4);
	static Vec3 GetPosition_Vec2(float x, float z);


};
