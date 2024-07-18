#include "pch.h"
#include "BTTask.h"



/* Script */



BTNodeState BTTask::Evaluate()
{
	return BTNodeState();
}

BTTask::BTTask(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback)
	: BTNode_Action(owner, callback)
{
	mType = type;
	
}

BTTask::~BTTask()
{
}

