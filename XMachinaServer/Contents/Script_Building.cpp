#include "pch.h"
#include "Script_Building.h"

Script_Building::Script_Building()
{
}

Script_Building::Script_Building(SPtr<GameObject> owner, ScriptInfo::Type type)
{
}

Script_Building::~Script_Building()
{
}

void Script_Building::Clone(SPtr<Component> other) 
{
    Script::Clone(other);

    SPtr<Script_Building> otherScript = std::static_pointer_cast<Script_Building>(other);

    this->mSectorIdx = otherScript->mSectorIdx;
}

void Script_Building::Activate()
{
    Script::Activate();
}

void Script_Building::DeActivate()
{
    Script::DeActivate();
}

bool Script_Building::WakeUp()
{
    Script::WakeUp();
    return true;
}

bool Script_Building::Start()
{
    Script::Start();
    return true;
}

bool Script_Building::Update()
{
    Script::Update();
    return true;
}

void Script_Building::OnDestroy()
{
    Script::OnDestroy();
}
