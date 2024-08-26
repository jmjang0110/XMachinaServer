#include "pch.h"
#include "Script_Player.h"

#include "GameObject.h"
#include "GameSession.h"
#include "GameRoom.h"
#include "RoomManager.h"
#include "Transform.h"
#include "ViewList.h"

#include "FBsPacketFactory.h"

#include "Script_Skill.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
#include "Script_Stat.h"

Script_Player::Script_Player()
    : Script_PlayerStat()
{
    Script_Entity::EnableTag(EntityTag::Player);

}

Script_Player::Script_Player(SPtr<GameObject> owner)
    : Script_PlayerStat(owner)
{
    Script_Entity::EnableTag(EntityTag::Player);
    owner->EnableTag(ObjectTag::Player);


}
Script_Player::~Script_Player()
{
    mSessionOwner = nullptr;
    for (int i = 0; i < mSkills.size(); ++i) {
        mSkills[i] = nullptr;
    }
    mViewList.Clear();

}
SPtr<Component> Script_Player::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Player
    auto clonedScript = std::dynamic_pointer_cast<Script_Player>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_PlayerStat::Clone(clonedScript);
        // Deep copy of mSkills if necessary
        for (size_t i = 0; i < mSkills.size(); ++i)
        {
            clonedScript->mSkills[i] = this->mSkills[i]->Clone(); // Assuming SPtr<GameObject> handles deep copying
        }
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Player" << std::endl;
        return nullptr;
    }
}
void Script_Player::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Player instance to the GameObject
    auto clonedScript = target->AddScript<Script_Player>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}

void Script_Player::Start()
{
    mViewList.ViewRangeRadius = 45.f;
}

void Script_Player::UpdateViewList(std::vector<SPtr<GameObject>> players, std::vector<SPtr<GameObject>> monster)
{
	ViewList PrevViewList;
	ViewList currViewList;

	mViewList_Lock.LockRead();
	PrevViewList = mViewList;
	mViewList_Lock.UnlockRead();

	std::vector<SPtr<GameObject>> NewMonsters;
	std::vector<SPtr<GameObject>> RemoveMonsters;

	/// +--------------------------------------------------------------------------------
	///	1. [PLAYER] VIEW LIST 
	/// ---------------------------------------------------------------------------------+
	for (int i = 0; i < players.size(); ++i) {
		currViewList.TryInsertPlayer(players[i]->GetID(), players[i]);
	}



	/// +--------------------------------------------------------------------------------
	///	2. [NEW MONSTER] VIEW LIST 
	/// ---------------------------------------------------------------------------------+
	for (int i = 0; i < monster.size(); ++i) {
		Script_Stat::ObjectState objState = monster[i]->GetScriptEntity<Script_Stat>()->S_GetObjectState();
		if (objState == Script_Stat::ObjectState::End)
			continue;

		bool IsSuccess = false;
		if (objState == Script_Stat::ObjectState::Dead)
			IsSuccess = currViewList.TryInsertMonster(monster[i]->GetID(), monster[i], false);
		else
			IsSuccess = currViewList.TryInsertMonster(monster[i]->GetID(), monster[i]);

		if (IsSuccess) {
			NewMonsters.push_back(monster[i]);
		}
	}

	std::unordered_set<int> currentMonsterIDs;
	for (int i = 0; i < monster.size(); ++i) {
		currentMonsterIDs.insert(monster[i]->GetID());
	}


	/// +--------------------------------------------------------------------------------
	///	3. [REMOVE MONSTER] VIEW LIST 
	/// ---------------------------------------------------------------------------------+
	for (auto& it : PrevViewList.VL_Monsters) {
		// ���� ViewList�� �ִ� Monster�� ���� ViewList�� ���ٸ� 
		if (currentMonsterIDs.find(it.first) == currentMonsterIDs.end()) {
			Script_Stat::ObjectState objState = it.second->GetScriptEntity<Script_Stat>()->S_GetObjectState();
			if (objState == Script_Stat::ObjectState::End)
				continue;

			bool DoDeactivate = true;
			if (objState == Script_Stat::ObjectState::Dead)
				DoDeactivate = false;

			Vec3	Monster_Pos = it.second->GetTransform()->GetPosition();
			Vec3	Player_Pos  = mOwner->GetTransform()->GetPosition();
			float	dist        = (Monster_Pos - Player_Pos).Length();

			if (dist > PrevViewList.ViewRangeRadius) {
				currViewList.RemoveMonster(it.first, DoDeactivate);
				RemoveMonsters.push_back(it.second);
			}
		}
	}


	/// +------------------------------------------------------------------------------------------------------
	/// 4. SEND NEW MOSNTERS PACKET 
	/// ------------------------------------------------------------------------------------------------------+
	if (NewMonsters.size() > 0) {

		auto NewMonster_spkt = FBS_FACTORY->SPkt_NewMonster(NewMonsters);
		mSessionOwner->Send(NewMonster_spkt);

		for (int i = 0; i < NewMonsters.size(); ++i) {

			Script_Stat::ObjectState objState = NewMonsters[i]->GetScriptEntity<Script_Stat>()->S_GetObjectState();
			if (Script_Stat::ObjectState::Dead == objState ||
				Script_Stat::ObjectState::End == objState)
				continue;

			/// > �� SEND MOSNTER STATE 
			auto						script = NewMonsters[i]->GetScript<Script_EnemyController>();
			FBProtocol::MONSTER_BT_TYPE btType = script->GetMonsterBTType();
			const auto& SPkt_monState          = FBS_FACTORY->SPkt_Monster_State(NewMonsters[i]->GetID(), btType);
			mSessionOwner->Send(SPkt_monState);

			/* TARGET PACKET */
			SPtr<GameObject> target = script->GetTarget(); /* Lock Read */
			int target_player_id = 0;
			int target_monster_id = 0;
			if (target && target->GetScriptEntity<Script_Entity>()->GetTag() == EntityTag::Player) {
				target_player_id = target->GetID();
			}
			else if (target) {
				target_monster_id = target->GetID();
			}

			/// > �� BROADCAST MOSNTER TRAGET 
			int monster_id = NewMonsters[i]->GetID();
			auto pkt = FBS_FACTORY->SPkt_Monster_Target(monster_id, target_player_id, target_monster_id);
			
			ROOM_MGR->BroadcastRoom(mOwner->GetOwnerRoom()->GetID(), pkt);
		}
	}

	/// +------------------------------------------------------------------------------------------------------
	/// 5. SEND REMOVE MOSNTERS PACKET 
	/// ------------------------------------------------------------------------------------------------------+
	for (int i = 0; i < RemoveMonsters.size(); ++i) {
		const auto& RemoveMonster_serverPacket = FBS_FACTORY->SPkt_RemoveMonster(RemoveMonsters[i]->GetID());
		mSessionOwner->Send(RemoveMonster_serverPacket);
	}


	mViewList_Lock.LockWrite();
	mViewList = currViewList;
	mViewList_Lock.UnlockWrite();
}

int Script_Player::OnShoot(Vec3& bullet_center, Vec3& bullet_dir)
{
    return 0;
}

int Script_Player::OnHitEnemy(int32_t monster_id, Vec3& bullet_center, Vec3& bullet_dir)
{
    return 0;
}

int Script_Player::OnHitExpEnemy(int32_t monster_id)
{
    return 0;
}

bool Script_Player::OnSkill(FBProtocol::PLAYER_SKILL_TYPE type, SPtr<GameObject> mindControlledMonster)
{
    return false;
}

SPtr<Script_Skill> Script_Player::GetSkillEntity(FBProtocol::PLAYER_SKILL_TYPE type)
{
    auto skill        = mSkills[type];
    auto skill_entity = skill->GetScriptEntity<Script_Skill>();
    return skill_entity;
}

