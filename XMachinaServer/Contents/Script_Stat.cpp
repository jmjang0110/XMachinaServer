#include "pch.h"
#include "Script_Stat.h"

Script_Stat::Script_Stat()
{
}

Script_Stat::Script_Stat(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script(owner, type, static_cast<UINT32>(type))
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


bool Script_Stat::Hit(float damage, SPtr_GameObject instigator)
{
	//LOG_MGR->Cout(" -- Script_Stat::Hit - HP : ", mCrntHP, "\n");

	if (mCrntHP <= 0) {
		return false;
	}

	if (mShieldAmount > 0) {
		mShieldAmount -= damage;
	}
	else {
		mCrntHP -= damage;
		S_SetHp(mCrntHP); // Lock Write
	}

	if (mCrntHP <= 0) {
		Dead();
		return true;
	}

	return false;
}

void Script_Stat::Dead()
{
	S_SetObjectState(ObjectState::Dead);

}


void Script_Stat::Clone(SPtr<Component> other) 
{
	Script::Clone(other);
	SPtr<Script_Stat> otherScript = std::static_pointer_cast<Script_Stat>(other);

}

void Script_Stat::Activate()
{
	Script::Activate();
	if(S_GetObjectState() == ObjectState::Deactive)
		S_SetObjectState(ObjectState::Active);

}

void Script_Stat::DeActivate()
{
	Script::DeActivate();

	if(S_GetObjectState() == ObjectState::Active)
		S_SetObjectState(ObjectState::Deactive);

}

