#pragma once

#include "Script_BehaviorTree.h"
#include "BTNode.h"
#include "Gameinfo.h"

class Script_Enemy;

class Script_DefaultEnemyBT : public Script_BehaviorTree
{
private:
	BTNode*					mRoot;
	Script_EnemyController* mEnemyController;


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
	virtual void Clone(SPtr<Component> other) ;

	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();

protected:
	virtual BTNode* SetupTree() override;

	SPtr<Script_Enemy> GetScriptEnemy(GameObjectInfo::Type objtype);


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
