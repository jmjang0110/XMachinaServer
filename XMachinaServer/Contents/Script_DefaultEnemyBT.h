#pragma once

#include "Script_BehaviorTree.h"
#include "BTNode.h"

class Script_DefaultEnemyBT : public Script_BehaviorTree
{
private:

public:
public:
	Script_DefaultEnemyBT();
	Script_DefaultEnemyBT(SPtr<GameObject> owner, ScriptInfo::Type type);
	virtual ~Script_DefaultEnemyBT();

public:
	virtual void OnEnable() override ;
	virtual void OnDisable() override;

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
	virtual BTNode* SetupTree() override;

public:
	template<typename... Args>
	std::vector<BTNode*> CreateChildren(Args... args);
};

template<typename ...Args>
inline std::vector<BTNode*> Script_DefaultEnemyBT::CreateChildren(Args ...args)
{
	std::vector<BTNode*> selectorNode;
	(selectorNode.push_back(args), ...);
	return selectorNode;
}
