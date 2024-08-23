#pragma once

#include "Script.h"
#include "BTNode.h"

class Script_BehaviorTree : public Script
{
private:
	BTNode* mRoot{};

public:
	Script_BehaviorTree();
	Script_BehaviorTree(SPtr<GameObject> owner);
	virtual ~Script_BehaviorTree();

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);
public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Start();
	virtual void Update();


protected:
	virtual BTNode* SetupTree() abstract;


};

