#include "pch.h"
#include "Script_BehaviorTree.h"


Script_BehaviorTree::Script_BehaviorTree()
{
}

Script_BehaviorTree::Script_BehaviorTree(SPtr<GameObject> owner, ScriptInfo::Type type)
    : Script(owner, type, static_cast<UINT32>(type))
{
    SAFE_DELETE(mRoot);
}

Script_BehaviorTree::~Script_BehaviorTree()
{
}

void Script_BehaviorTree::Clone(SPtr<Component> other) 
{
    Script::Clone(other);

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

    mRoot = SetupTree();
    mRoot->SetRoot();

    return true;
}

bool Script_BehaviorTree::Update()
{
    Script::Update();

    if (mRoot) {
        mRoot->Evaluate();
    }

    return true;
}

void Script_BehaviorTree::OnDestroy()
{
    Script::OnDestroy();

}
