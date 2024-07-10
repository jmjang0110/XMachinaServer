#include "pch.h"
#include "Script_BehaviorTree.h"


Script_BehaviorTree::Script_BehaviorTree()
{
}

Script_BehaviorTree::Script_BehaviorTree(UINT32 id)
    : Script(ScriptInfo::Type::BehaviorTree, id)
{
}

Script_BehaviorTree::~Script_BehaviorTree()
{
}

void Script_BehaviorTree::Activate()
{
    Script::Activate();

}

void Script_BehaviorTree::DeActivate()
{
    Script::DeActivate();

}

bool Script_BehaviorTree::WakeUp()
{
    Script::WakeUp();

    return true;
}

bool Script_BehaviorTree::Start()
{
    Script::Start();

    return true;
}

bool Script_BehaviorTree::Update()
{
    Script::Update();

    return true;
}

void Script_BehaviorTree::OnDestroy()
{
    Script::OnDestroy();

}
