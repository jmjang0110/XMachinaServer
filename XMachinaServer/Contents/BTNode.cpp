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

BTNodeState Sequence_BTNode::Evaluate()
{
    return BTNodeState();
}

BTNodeState Action_BTNode::Evaluate()
{
    return BTNodeState();
}

BTNodeState Selector_BTNode::Evaluate()
{
    return BTNodeState();
}
