#pragma once

#include "Script_BehaviorTree.h"
#include "BTNode.h"
#include "Gameinfo.h"

class Script_Enemy;
class Script_MindControlledEnemy : public Script_BehaviorTree
{
private:
	BTNode*				mRoot             = {};
	SPtr<GamePlayer>	mInvoker          = {};
	float				mShieldBuffAmount = {};

public:
	Script_MindControlledEnemy();
	Script_MindControlledEnemy(SPtr<GameObject> owner, ScriptInfo::Type type);
	virtual ~Script_MindControlledEnemy();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other);
	virtual bool Update();


	void SetInvoker(SPtr<GamePlayer> invoker) { mInvoker = invoker; }
protected:
	virtual BTNode* SetupTree() override;

	SPtr<Script_Enemy> GetScriptEnemy(GameObjectInfo::Type objtype);


public:
	template<typename... Args>
	std::vector<BTNode*> CreateChildren(Args... args);
};

template<typename ...Args>
inline std::vector<BTNode*> Script_MindControlledEnemy::CreateChildren(Args ...args)
{
	return std::vector<BTNode*>();
}
