#pragma once

#include "Script_BehaviorTree.h"
#include "BTNode.h"
#include "Gameinfo.h"

class Script_Enemy;

class Script_DefaultEnemyBT : public Script_BehaviorTree
{
private:
	BTNode*					mRoot;

public:
	Script_DefaultEnemyBT();
	Script_DefaultEnemyBT(SPtr<GameObject> owner, ScriptInfo::Type type);
	virtual ~Script_DefaultEnemyBT();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other) ;
	virtual bool Update();


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
