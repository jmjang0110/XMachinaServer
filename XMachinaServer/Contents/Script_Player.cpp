#include "pch.h"
#include "Script_Player.h"

#include "GameObject.h"
#include "ViewList.h"
#include "Script_Skill.h"

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

void Script_Player::UpdateViewList(std::vector<SPtr<GameObject>> players, std::vector<SPtr<GameObject>> montser)
{
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

