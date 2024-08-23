#include "pch.h"
#include "BTNode.h"

#include "Script_EnemyController.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Animation.h"



BTNode::BTNode(SPtr<GameObject> owner)
{
	mOwner = owner;
}

BTNode::BTNode(SPtr<GameObject> owner, std::vector<BTNode*>& children)
{
	mOwner = owner;

	for (auto& child : children) {
		Attach(child);
	}

	mEnemyController = owner->GetScriptEntity<Script_EnemyController>();
}

BTNode::~BTNode()
{
	for (int i = 0; i < mChildren.size(); ++i) {
		MEMORY->Delete(mChildren[i]);
	}
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
///	> ������ BT Node Sequence
/// __________________________________________________________________________

BTNodeState BTNode_Sequence::Evaluate()
{
	//LOG_MGR->Cout("BTNode_Sequence \n");

	bool isRunning = false;

	if (!mRoot->mWaitQueue.empty()) {
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
		case BTNodeState::Success:
			continue;
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
///	> ������ BT Node Selector  
/// __________________________________________________________________________
BTNodeState BTNode_Selector::Evaluate()
{
	//LOG_MGR->Cout("BTNode_Selector \n");

	if (!mRoot->mWaitQueue.empty()) {
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
			continue;
		}
	}
	return BTNodeState::Failure;

}

