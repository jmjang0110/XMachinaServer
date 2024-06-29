#include "pch.h"
#include "BTNode.h"

BTNode::BTNode()
{
}

BTNode::BTNode(std::vector<BTNode*>& child)
{
}

BTNode::~BTNode()
{
}

void BTNode::SetRoot()
{
}

void BTNode::Attach(BTNode* node)
{
}

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º BT Node Sequence
/// __________________________________________________________________________

BTNodeState BTNode_Sequence::Evaluate()
{
	return BTNodeState();
}

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º BT Node Selector  
/// __________________________________________________________________________
BTNodeState BTNode_Selector::Evaluate()
{
	return BTNodeState();
}


/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º BT Node Action  
/// __________________________________________________________________________

BTNodeState BTNode_Action::Evaluate()
{
	return BTNodeState();
}
