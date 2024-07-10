#pragma once


enum class BTNodeState {
	Running = 0,
	Success,
	Failure,
	Wait,
};

/// +-------------------------------------------------------------------------
///	> ▷▷▷ Behavior Tree Node 
/// -------------------------------------------------------------------------+

class BTNode
{
public:
	SPtr_GameObject			mOwner          = {};

	BTNode*					mRoot           = {};
	BTNode*					mParent         = {};

	std::queue<BTNode*>		mWaitQueue  = {};
	std::vector<BTNode*>	mChildren   = {};

public:
	BTNode(SPtr_GameObject owner);
	BTNode(SPtr_GameObject owner, std::vector<BTNode*>& children);
	virtual ~BTNode();

public:
	virtual BTNodeState Evaluate() { return BTNodeState::Failure; }
	void SetRoot();

	SPtr_GameObject GetOwner() { return mOwner; }
	BTNode*			GetRoot() { return mRoot; }
	BTNode*			GetParent() { return mParent; }

public:
	void SetOwner(SPtr_GameObject owner) { mOwner = owner; }
	void SetRoot(BTNode* root) { mRoot = root; }
	void SetParent(BTNode* parent) { mParent = parent; }



private:
	void Attach(BTNode* node);

};

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Sequence Tree Node 
/// __________________________________________________________________________
/// - 자식 노드들을 순서대로 실행하기 위한 노드
/// - Evaluate 에서 자식노드를 왼쪽에서 오른쪽으로 DFS방식으로 평가 
/// -------------------------------------------------------------------------+

class BTNode_Sequence : public BTNode 
{
public:
	virtual BTNodeState Evaluate() override;

public:
	BTNode_Sequence(SPtr_GameObject owner) : BTNode(owner) {};
	BTNode_Sequence(SPtr_GameObject owner, std::vector<BTNode*>& children) : BTNode(owner, children) {};
};

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Selector Tree Node 
/// __________________________________________________________________________
/// - 자식 노드들 중 하나를 행하기 위한 노드
/// - Evaluate 에서 자식 노드를 왼쪽에서 오른쪽으로 DFS 방식으로 평가 
/// -------------------------------------------------------------------------+

class BTNode_Selector : public BTNode
{
public:
	virtual BTNodeState Evaluate() override;

public:
	BTNode_Selector(SPtr_GameObject owner) : BTNode(owner) {};
	BTNode_Selector(SPtr_GameObject owner, std::vector<BTNode*>& children) : BTNode(owner, children) {};
};

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Action Tree Node 
/// __________________________________________________________________________
/// mCallback 함수를 실행시켜 Action을 취한다. 
/// -------------------------------------------------------------------------+


class BTNode_Action : public BTNode
{
private:
	std::function<void()> mCallback;

public:
	virtual BTNodeState Evaluate() { return BTNode::Evaluate(); };

public:
	BTNode_Action(SPtr_GameObject owner) : BTNode(owner) {};
	BTNode_Action(SPtr_GameObject owner, std::function<void()> callback) : BTNode(owner) { mCallback = callback; }

public:
	void ExecuteCallback() {
		if (mCallback)
			mCallback();
	}
};