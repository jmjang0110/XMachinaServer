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
protected:
	SPtr_NetObj				Owner      = {};
	
	BTNode*					Root       = {};
	BTNode*					Parent     = {};

	std::queue<BTNode*>		WaitQueue  = {};
	std::vector<BTNode*>	children   = {};

public:
	BTNode();
	BTNode(std::vector<BTNode*>& children);
	virtual ~BTNode();

public:
	virtual BTNodeState Evaluate() { return BTNodeState::Failure; }
	void SetRoot();

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
	BTNode_Sequence() : BTNode() {};
	BTNode_Sequence(std::vector<BTNode*>& children) : BTNode(children) {};
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
	BTNode_Selector() : BTNode() {};
	BTNode_Selector(std::vector<BTNode*>& children) : BTNode(children) {};
};

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Action Tree Node 
/// __________________________________________________________________________
/// - Action Node를 상속받는다는 것은 
/// -------------------------------------------------------------------------+


class BTNode_Action : public BTNode
{
private:
	std::function<void()> mCallback;

public:
	virtual BTNodeState Evaluate() override;

public:
	BTNode_Action() : BTNode() {};
	BTNode_Action(std::function<void()> callback) : mCallback(callback) {}
};