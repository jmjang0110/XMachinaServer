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

class BTNode_Sequence : public BTNode 
{
public:
	virtual BTNodeState Evaluate() override;

public:
	BTNode_Sequence() : BTNode() {};
	BTNode_Sequence(std::vector<BTNode*>& children) : BTNode(children) {};
};

/// +-------------------------------------------------------------------------
///	> ������ Selector Tree Node 
/// __________________________________________________________________________
/// - �ڽ� ���� �� �ϳ��� ���ϱ� ���� ���
/// - Evaluate ���� �ڽ� ��带 ���ʿ��� ���������� DFS ������� �� 
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
///	> ������ Action Tree Node 
/// __________________________________________________________________________
/// - Action Node�� ��ӹ޴´ٴ� ���� 
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