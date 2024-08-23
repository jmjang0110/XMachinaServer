#include "pch.h"
#include "Script_PlayerStat.h"
#include "GameObject.h"
#include "Transform.h"


Script_PlayerStat::Script_PlayerStat()
{


}

Script_PlayerStat::Script_PlayerStat(SPtr<GameObject> owner)
	: Script_Stat(owner, uint32_t_ScriptKey(ScriptKey::Player))
{
}

Script_PlayerStat::~Script_PlayerStat()
{
}

SPtr<Component> Script_PlayerStat::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_PlayerStat
	auto clonedScript = std::dynamic_pointer_cast<Script_PlayerStat>(target);
	if (clonedScript)
	{
		// Call the base class Clone method first
		Script_Stat::Clone(clonedScript);

		// Copy the specific member variables
		clonedScript->mRespawn_Position = this->mRespawn_Position;
		clonedScript->mScore            = this->mScore;

		clonedScript->mStartPheroAmount = this->mStartPheroAmount;
		clonedScript->mMaxPheroAmount   = this->mMaxPheroAmount;
		clonedScript->mCurrPheroAmount  = this->mCurrPheroAmount; // Handle lock if needed
		clonedScript->mPheroRegenRate   = this->mPheroRegenRate;

		clonedScript->mVelocity			= this->mVelocity;

		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_PlayerStat" << std::endl;
		return nullptr;
	}
}

void Script_PlayerStat::Clone(SPtr<GameObject> target)
{
	// Add a new Script_PlayerStat instance to the GameObject
	auto clonedScript = target->AddScript<Script_PlayerStat>();
	// Clone the current script into the new script
	this->Clone(clonedScript);
}

void Script_PlayerStat::Start()
{
	Script_Stat::Start();
	Script_Stat::SetMaxHP(1000000.f); // TODO : Test HP 
}

void Script_PlayerStat::SetSpawn(const Vec3& pos)
{
	mRespawn_Position = pos;

	mOwner->GetTransform()->SetPosition(mRespawn_Position);
	mOwner->GetTransform()->UpdateTransofrmSnapShot();
	mOwner->GetTransform()->UpdateTransofrmSnapShot();
}



/// +-------------------------------------------
///	 >> Phero
/// -------------------------------------------+
void Script_PlayerStat::AddPheroAmount(float pheroAmount)
{
	float currphero = S_GetCurrPheroAmount();
	currphero -= pheroAmount;
	currphero = min(currphero, mMaxPheroAmount);
	S_SetCurrPheroAmount(currphero);
}

bool Script_PlayerStat::ReducePheroAmount(float pheroCost)
{
	float currphero = S_GetCurrPheroAmount();
	currphero -= pheroCost;
	if (currphero < 0)
		return false;
	else
		S_SetCurrPheroAmount(currphero);
	return true;
}

void Script_PlayerStat::S_SetCurrPheroAmount(float phero)
{
	Lock_CurrPheroAmount.LockWrite();
	mCurrPheroAmount = phero;
	Lock_CurrPheroAmount.UnlockWrite();

}

float Script_PlayerStat::S_GetCurrPheroAmount()
{
	float currPhero = 0.f;
	Lock_CurrPheroAmount.LockRead();
	currPhero = mCurrPheroAmount;
	Lock_CurrPheroAmount.UnlockRead();
	return currPhero;
}
