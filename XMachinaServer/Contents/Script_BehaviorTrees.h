#pragma once

#include "Script_BehaviorTree.h"
#include "BTNode.h"

class Script_Enemy;

class Script_DefaultEnemyBT : public Script_BehaviorTree
{
protected:
	BTNode* mRoot{};
	SPtr<GameObject> mPrevTarget{};

public:
	Script_DefaultEnemyBT(SPtr<GameObject> owner) : Script_BehaviorTree(owner) {}
	virtual ~Script_DefaultEnemyBT() { MEMORY->Delete(mRoot); }

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);
	virtual void Update();

	void PrintBTtype(FBProtocol::MONSTER_BT_TYPE bttype, std::string substring = "");


protected:
	virtual BTNode* SetupTree() override;
};

class Script_MindControlledEnemyBT : public Script_DefaultEnemyBT
{
public:
	Script_MindControlledEnemyBT(SPtr<GameObject> owner) : Script_DefaultEnemyBT(owner) {}

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);
protected:
	virtual BTNode* SetupTree() override;
};

class Script_DeusPhase1BT : public Script_DefaultEnemyBT
{
public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);
	Script_DeusPhase1BT(SPtr<GameObject> owner) : Script_DefaultEnemyBT(owner) {}

protected:
	virtual BTNode* SetupTree() override;
};