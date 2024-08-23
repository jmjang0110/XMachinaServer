#include "pch.h"
#include "Script_BehaviorTree.h"
#include "GameObject.h"


Script_BehaviorTree::Script_BehaviorTree()
{
}

Script_BehaviorTree::Script_BehaviorTree(SPtr<GameObject> owner)
    : Script(owner, uint32_t_ScriptKey(ScriptKey::BehaviorTree))
{
    SAFE_DELETE(mRoot);
}

Script_BehaviorTree::~Script_BehaviorTree()
{
}

SPtr<Component> Script_BehaviorTree::Clone(SPtr<Component> target)
{
    // Attempt to cast target to Script_BehaviorTree
    auto clonedScript = std::dynamic_pointer_cast<Script_BehaviorTree>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_BehaviorTree" << std::endl;
        return nullptr;
    }
}

void Script_BehaviorTree::Clone(SPtr<GameObject> target)
{
}

void Script_BehaviorTree::Start()
{
    Script::Start();

    mRoot = SetupTree();
    mRoot->SetRoot();

}

void Script_BehaviorTree::Update()
{
    Script::Update();

    if (mRoot) {
        mRoot->Evaluate();
    }

}
