#pragma once

#include "Script_BehaviorTree.h"
#include "BTNode.h"
#include "Gameinfo.h"

class Script_Enemy;

class Script_DefaultEnemyBT : public Script_BehaviorTree
{
protected:
	BTNode* mRoot{};
	SPtr_GameObject mPrevTarget{};

public:
	Script_DefaultEnemyBT(SPtr<GameObject> owner, ScriptInfo::Type type) : Script_BehaviorTree(owner, type) {}
	virtual ~Script_DefaultEnemyBT() { MEMORY->Delete(mRoot); }

public:
	virtual void Clone(SPtr<Component> other);
	virtual bool Update();

protected:
	virtual BTNode* SetupTree() override;
};

class Script_MindControlledEnemyBT : public Script_DefaultEnemyBT
{
public:
	Script_MindControlledEnemyBT(SPtr<GameObject> owner, ScriptInfo::Type type) : Script_DefaultEnemyBT(owner, type) {}

protected:
	virtual BTNode* SetupTree() override;
};

class Script_DeusPhase1BT : public Script_DefaultEnemyBT
{
public:
	Script_DeusPhase1BT(SPtr<GameObject> owner, ScriptInfo::Type type) : Script_DefaultEnemyBT(owner, type) {}

protected:
	virtual BTNode* SetupTree() override;
};