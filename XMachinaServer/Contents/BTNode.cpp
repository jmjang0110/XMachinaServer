#include "pch.h"
#include "BTNode.h"

BTNode::BTNode()
{
}

BTNode::BTNode(std::vector<BTNode*>& children)
{
	for (auto& child : children) {
		Attach(child);
	}
}

BTNode::~BTNode()
{
}

void BTNode::SetRoot()
{
	mRoot = this;

	std::queue<BTNode*> Q;
	Q.push(mRoot);

	while (!Q.empty()) {
		BTNode* currNode = Q.front();
		Q.pop();

		for (auto& child : currNode->mChildren) {
			child->mRoot = this;
			Q.push(child);
		}
	}


}

void BTNode::Attach(BTNode* node)
{
	node->mParent = this;
	mChildren.emplace_back(node);

}

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º BT Node Sequence
/// __________________________________________________________________________

BTNodeState BTNode_Sequence::Evaluate()
{
	bool isRunning = false;

	if (!GetRoot()->mWaitQueue.empty()) {
		if (mRoot->mWaitQueue.front()->Evaluate() != BTNodeState::Wait) {
			mRoot->mWaitQueue.pop();
		}

		return BTNodeState::Success;
	}

	for (const auto& child : mChildren) {
		switch (child->Evaluate())
		{
		case BTNodeState::Failure:
			return BTNodeState::Failure;
		case BTNodeState::Running:
			isRunning = true;
			continue;
		case BTNodeState::Wait:
			mRoot->mWaitQueue.push(child);
			return BTNodeState::Success;
		default:
			return BTNodeState::Success;
		}
	}

	return isRunning ? BTNodeState::Running : BTNodeState::Success;

}

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º BT Node Selector  
/// __________________________________________________________________________
BTNodeState BTNode_Selector::Evaluate()
{
	if (mRoot->mWaitQueue.empty()) {
		if (mRoot->mWaitQueue.front()->Evaluate() != BTNodeState::Wait)
			mRoot->mWaitQueue.pop();
		
		return BTNodeState::Failure;
	}

	for (const auto& child : mChildren) {
		switch (child->Evaluate())
		{
		case BTNodeState::Failure:
			continue;
		case BTNodeState::Success:
			return BTNodeState::Success;
		case BTNodeState::Running:
			return BTNodeState::Running;
		case BTNodeState::Wait:
			mRoot->mWaitQueue.push(child);
			return BTNodeState::Success;

		default:
			break;
		}
	}
	return BTNodeState::Failure;

}


/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º BT Node Action  
/// __________________________________________________________________________

BTNodeState BTNode_Action::Evaluate()
{
	return BTNodeState();
}
