#pragma once


enum class BTNodeState {
	Running = 0,
	Success,
	Failure,
	Wait,
};

/// +-------------------------------------------------------------------------
///	> ������ Behavior Tree Node 
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
///	> ������ Sequence Tree Node 
/// __________________________________________________________________________
/// - �ڽ� ������ ������� �����ϱ� ���� ���
/// - Evaluate ���� �ڽĳ�带 ���ʿ��� ���������� DFS������� �� 
/// -------------------------------------------------------------------------+

class Sequence_BTNode : public BTNode {
public:
	virtual BTNodeState Evaluate() override;

public:
	Sequence_BTNode() : BTNode() {};
	Sequence_BTNode(std::vector<BTNode*>& children) : BTNode(children) {};
};

/// +-------------------------------------------------------------------------
///	> ������ Selector Tree Node 
/// __________________________________________________________________________
/// - �ڽ� ���� �� �ϳ��� ���ϱ� ���� ���
/// - Evaluate ���� �ڽ� ��带 ���ʿ��� ���������� DFS ������� �� 
/// -------------------------------------------------------------------------+

class Selector_BTNode : public BTNode {
public:
	virtual BTNodeState Evaluate() override;

public:
	Selector_BTNode() : BTNode() {};
	Selector_BTNode(std::vector<BTNode*>& children) : BTNode(children) {};
};

/// +-------------------------------------------------------------------------
///	> ������ Action Tree Node 
/// __________________________________________________________________________
/// - �ڽ� ���� �� �ϳ��� ���ϱ� ���� ���
/// - Evaluate ���� �ڽ� ��带 ���ʿ��� ���������� DFS ������� �� 
/// -------------------------------------------------------------------------+


class Action_BTNode : public BTNode {
public:
	virtual BTNodeState Evaluate() override;

public:
	Action_BTNode() : BTNode() {};
	Action_BTNode(std::vector<BTNode*>& children) : BTNode(children) {};
};