#include "pch.h"
#include "Script_Stat.h"
#include "GameObject.h"

Script_Stat::Script_Stat()
{
}

Script_Stat::Script_Stat(SPtr<GameObject> owner, UINT32 key)
	: Script_Entity(owner, key)
{
}

Script_Stat::~Script_Stat()
{
}

void Script_Stat::Start()
{
    Script::Start();

	mCrntHP = mMaxHP;
	mPrevHP = mMaxHP;
}


bool Script_Stat::Hit(float damage, SPtr<GameObject> instigator)
{
	LOG_MGR->Cout(mOwner->GetID(), "  : -- Script_Stat::Hit - HP : ", mCrntHP, "\n");

	if (mCrntHP <= 0) {
		Dead();
		return true;
	}

	if (mShieldAmount > 0) {
		mShieldAmount -= damage;
	}
	else {
		mCrntHP -= damage;
		LOG_MGR->Cout("ID : ", mOwner->GetID(), " HP : ", mCrntHP, "\n");
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


SPtr<Component> Script_Stat::Clone(SPtr<Component> target)
{
	// First, clone the base Script_Entity part
	Script_Entity::Clone(target);

	// Cast the target to the appropriate type (Script_Stat)
	auto statScript = std::dynamic_pointer_cast<Script_Stat>(target);

	// Ensure the casting was successful
	if (statScript)
	{
		// Copy the Script_Stat-specific member variables
		statScript->mMaxHP        = this->mMaxHP;
		statScript->mShieldAmount = this->mShieldAmount;
		statScript->mCrntHP       = this->mCrntHP;
		statScript->mPrevHP       = this->mPrevHP;
		statScript->mMaxPhero     = this->mMaxPhero;
		statScript->mPhero        = this->mPhero;

		// Copy the state and locks
		statScript->mObjectState = this->mObjectState;
		statScript->mHP          = this->mHP;

		// Locks are generally not copied, as the locks should be managed independently in the cloned object
		// If copying is necessary, you would need to create new lock objects (depending on your locking mechanism)
	}

	return target;
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

/// +-------------------------------------------
///	 >> Phero
/// -------------------------------------------+
void Script_Stat::AddPheroAmount(float pheroAmount)
{
	float currphero = S_GetCurrPheroAmount();
	currphero += pheroAmount;
	currphero = min(currphero, mMaxPhero);
	S_SetCurrPheroAmount(currphero);
}

bool Script_Stat::ReducePheroAmount(float pheroCost)
{
	float currphero = S_GetCurrPheroAmount();
	currphero -= pheroCost;
	if (currphero < 0)
		return false;
	else
		S_SetCurrPheroAmount(currphero);
	return true;
}

void Script_Stat::S_SetCurrPheroAmount(float phero)
{
	Lock_Phero.LockWrite();
	mPhero = phero;
	Lock_Phero.UnlockWrite();

}

float Script_Stat::S_GetCurrPheroAmount()
{
	float currPhero = 0.f;
	Lock_Phero.LockRead();
	currPhero = mPhero;
	Lock_Phero.UnlockRead();
	return currPhero;
}
