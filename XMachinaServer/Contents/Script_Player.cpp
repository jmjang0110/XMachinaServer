#include "pch.h"
#include "Script_Player.h"
#include "GamePlayer.h"

Script_Player::Script_Player()
    : Script_PlayerStat()
{
}

Script_Player::Script_Player(SPtr<GamePlayer> owner, ScriptInfo::Type type)
    : Script_PlayerStat(owner, type)
{
    for (int i = 0; i < static_cast<UINT8>(SkillInfo::Type::End); ++i) {
        if (mSkills[i] == nullptr) {
            SkillInfo::Type t = static_cast<SkillInfo::Type>(i);

            switch (t)
            {
            case SkillInfo::Type::Cloaking:
                mSkills[i] = new Skill(std::dynamic_pointer_cast<GamePlayer>(GetOwner()), t, 3.f, 0.f /* ¹«ÇÑ */);
                break;
            case SkillInfo::Type::MindControl:
                mSkills[i] = new Skill(std::dynamic_pointer_cast<GamePlayer>(GetOwner()), t, 10.f, 30.f);
                break;
            case SkillInfo::Type::IRDetector:
                mSkills[i] = new Skill(std::dynamic_pointer_cast<GamePlayer>(GetOwner()), t, 2.f, 10.f) ;
                break;
            case SkillInfo::Type::Shield:
                mSkills[i] = new Skill(std::dynamic_pointer_cast<GamePlayer>(GetOwner()), t, 2.f, 4.5f );
                break;
            }
        }
    }

}

Script_Player::~Script_Player()
{
    for (int i = 0; i < static_cast<UINT8>(SkillInfo::Type::End); ++i) {
        SAFE_DELETE(mSkills[i]);
    }

}

void Script_Player::Activate()
{
    Script_PlayerStat::Activate();

}

void Script_Player::DeActivate()
{
    Script_PlayerStat::DeActivate();

}

bool Script_Player::WakeUp()
{
    return false;
}

bool Script_Player::Start()
{
    return false;
}

bool Script_Player::Update()
{
    return false;
}

void Script_Player::OnDestroy()
{
}

void Script_Player::Attack()
{
}

void Script_Player::Dead()
{
}

bool Script_Player::Hit(float damage, SPtr_GameObject instigator)
{
    return false;
}
