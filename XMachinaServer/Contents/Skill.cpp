#include "pch.h"
#include "Skill.h"

Skill::Skill()
{

}

Skill::Skill(SPtr<GamePlayer> owner, SkillInfo::Type type, float cooltime, float activeduration)
		: mOwner(owner)
		, mType(type)
		, mCoolTime(cooltime)
		, mActiveDuration(activeduration)
{

}

Skill::~Skill()
{
	mOwner = nullptr;
}

void Skill::Update()
{

}
