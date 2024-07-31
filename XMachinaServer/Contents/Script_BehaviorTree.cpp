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
