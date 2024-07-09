#pragma once

#include "Script.h"
#include "BTNode.h"

class Script_BehaviorTree : public Script
{
private:
	BTNode* mRoot{};

public:
	Script_BehaviorTree();
	Script_BehaviorTree(UINT32 id);
	virtual ~Script_BehaviorTree();

public:
	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();


protected:
	virtual BTNode* SetupTree() abstract;


};

