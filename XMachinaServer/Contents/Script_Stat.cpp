#include "pch.h"
#include "Script_Stat.h"

Script_Stat::Script_Stat()
{
}

Script_Stat::Script_Stat(UINT32 id)
{
}

Script_Stat::~Script_Stat()
{
}

bool Script_Stat::WakeUp()
{
    Script::WakeUp();

	mCrntHP = mMaxHP;
	mPrevHP = mMaxHP;
    return true;
}

bool Script_Stat::Start()
{
    Script::Start();

	mCrntHP = mMaxHP;
	mPrevHP = mMaxHP;
    return true;
}

bool Script_Stat::Update()
{
    Script::Update();

    return true;
}

void Script_Stat::OnDestroy()
{
}

bool Script_Stat::Hit(float damage, SPtr_GameObject instigator)
{
	if (mCrntHP <= 0) {
		return false;
	}

	if (mShieldAmount > 0) {
		mShieldAmount -= damage;
	}
	else {
		mCrntHP -= damage;
	}

	if (mCrntHP <= 0) {
		Dead();
		return true;
	}

	return false;
}

void Script_Stat::Dead()
{
	mIsDead = true;
}

void Script_Stat::Attack()
{
}

void Script_Stat::Activate()
{
}

void Script_Stat::DeActivate()
{
}
