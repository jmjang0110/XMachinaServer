#pragma once

#include "Script_BehaviorTree.h"
#include "BTNode.h"

class Script_DefaultEnemyBT : public Script_BehaviorTree
{

private:


public:
	virtual void OnEnable() override ;
	virtual void OnDisable() override;

protected:
	virtual BTNode* SetupTree() override;

};

