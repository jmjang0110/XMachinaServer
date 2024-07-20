#include "pch.h"
#include "Script_Phero.h"

Script_Phero::Script_Phero()
{
}

Script_Phero::Script_Phero(SPtr<GameObject> owner, ScriptInfo::Type type)
    : Script(owner, type, static_cast<UINT32>(type))
{
}

Script_Phero::~Script_Phero()
{
}

SPtr<Component> Script_Phero::Clone(SPtr<GameObject> copyOwner) const
{
    return SPtr<Component>();
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
