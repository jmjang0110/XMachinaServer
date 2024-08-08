#include "pch.h"
#include "Script_Phero.h"

Script_Phero::Script_Phero()
{
    mTargetPlayer.store(-1);
    
}

Script_Phero::Script_Phero(SPtr<GameObject> owner, ScriptInfo::Type type)
    : Script(owner, type, static_cast<UINT32>(type))
{
    mTargetPlayer.store(-1);

}

Script_Phero::~Script_Phero()
{
}

void Script_Phero::Clone(SPtr<Component> other) 
{
    Script::Clone(other);
    SPtr<Script_Phero> otherScript = std::static_pointer_cast<Script_Phero>(other);

    this->mLevel           = otherScript->mLevel;
    this->mAmount          = otherScript->mAmount;
    this->mLifeTime        = otherScript->mLifeTime;
    mTargetPlayer.store(-1);

}

void Script_Phero::Activate()
{

}

void Script_Phero::DeActivate()
{

}

bool Script_Phero::WakeUp()
{

    return true;
}

bool Script_Phero::Start()
{

    return true;
}

bool Script_Phero::Update()
{

    return true;
}

void Script_Phero::OnDestroy()
{
}

void Script_Phero::Init(int level, float lifeTime, float amount)
{
    mLevel    = level;
    mLifeTime = lifeTime;
    mAmount   = amount;

}



