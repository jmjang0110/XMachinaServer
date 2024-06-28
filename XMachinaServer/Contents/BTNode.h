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

class Sequence_BTNode : public BTNode {
public:
	virtual BTNodeState Evaluate() override;

public:
	Sequence_BTNode() : BTNode() {};
	Sequence_BTNode(std::vector<BTNode*>& children) : BTNode(children) {};
};

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Selector Tree Node 
/// __________________________________________________________________________
/// - 자식 노드들 중 하나를 행하기 위한 노드
/// - Evaluate 에서 자식 노드를 왼쪽에서 오른쪽으로 DFS 방식으로 평가 
/// -------------------------------------------------------------------------+

class Selector_BTNode : public BTNode {
public:
	virtual BTNodeState Evaluate() override;

public:
	Selector_BTNode() : BTNode() {};
	Selector_BTNode(std::vector<BTNode*>& children) : BTNode(children) {};
};

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Action Tree Node 
/// __________________________________________________________________________
/// - 자식 노드들 중 하나를 행하기 위한 노드
/// - Evaluate 에서 자식 노드를 왼쪽에서 오른쪽으로 DFS 방식으로 평가 
/// -------------------------------------------------------------------------+


class Action_BTNode : public BTNode {
public:
	virtual BTNodeState Evaluate() override;

public:
	Action_BTNode() : BTNode() {};
	Action_BTNode(std::vector<BTNode*>& children) : BTNode(children) {};
};