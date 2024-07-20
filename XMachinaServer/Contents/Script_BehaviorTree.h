#pragma once

#include "Script.h"
#include "BTNode.h"

class Script_BehaviorTree : public Script
{
private:
	BTNode* mRoot{};

public:
	Script_BehaviorTree();
	Script_BehaviorTree(SPtr<GameObject> owner, ScriptInfo::Type type);
	virtual ~Script_BehaviorTree();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual SPtr<Component> Clone(SPtr<GameObject> copyOwner) const;

	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();


protected:
	virtual BTNode* SetupTree() abstract;


};

