#include "pch.h"
#include "Script_PlayerStat.h"
#include "GamePlayer.h"


Script_PlayerStat::Script_PlayerStat()
{
	Script_Stat::SetMaxHP(1000000.f); // TODO : Test HP 


}

Script_PlayerStat::Script_PlayerStat(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Stat(owner, type)
{
	Script_Stat::SetMaxHP(1000000.f);


}

Script_PlayerStat::~Script_PlayerStat()
{
}

void Script_PlayerStat::Clone(SPtr<Component> other) 
{
	Script_Stat::Clone(other);
	SPtr<Script_PlayerStat> otherScript = std::static_pointer_cast<Script_PlayerStat>(other);

	mRespawn_Position	= otherScript->mRespawn_Position;// 리스폰 지점
	mScore				= otherScript->mScore;

	 mStartPheroAmount	= otherScript->mStartPheroAmount;
	 mCurrPheroAmount	= otherScript->mCurrPheroAmount;
	 mMaxPheroAmount	= otherScript->mMaxPheroAmount;
	 mPheroRegenRate	= otherScript->mPheroRegenRate;
						
	 mCrntWeaponNum		= otherScript->mCrntWeaponNum;
	 mNextWeaponNum		= otherScript->mNextWeaponNum;


}

void Script_PlayerStat::Activate()
{
	Script_Stat::Activate();

}

void Script_PlayerStat::DeActivate()
{
	Script_Stat::DeActivate();

}

bool Script_PlayerStat::WakeUp()
{
	Script_Stat::WakeUp();
	return true;
}

bool Script_PlayerStat::Start()
{
	Script_Stat::Start();

	mPlayerOwner = std::dynamic_pointer_cast<GamePlayer>(GetOwner());

	return true;
}

bool Script_PlayerStat::Update()
{
	Script_Stat::Update();
	return true;
}

void Script_PlayerStat::OnDestory()
{
	Script_Stat::OnDestroy();

}

bool Script_PlayerStat::Attack()
{
	return true;
}

void Script_PlayerStat::Dead()
{
	Script_Stat::Dead();

}

bool Script_PlayerStat::Hit(float damage, SPtr_GameObject instigator)
{
	Script_Stat::Hit(damage, instigator);
		
	return true;
}

void Script_PlayerStat::SetSpawn(const Vec3& pos)
{
	mRespawn_Position = pos;
	GetOwner()->GetTransform()->SetPosition(mRespawn_Position);

	GetOwner()->GetTransform()->UpdateTransofrmSnapShot();
	GetOwner()->GetTransform()->SwapSnapShotIndex();
	GetOwner()->GetTransform()->UpdateTransofrmSnapShot();
	GetOwner()->GetTransform()->SwapSnapShotIndex();
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
