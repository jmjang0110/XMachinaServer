#include "pch.h"
#include "NetworkObject.h"

NetworkObject::NetworkObject()
{

}

NetworkObject::~NetworkObject()
{
	mSocketData.Close();

}

void NetworkObject::PQCS(OverlappedObject* over)
{
	::PostQueuedCompletionStatus(over->GetIocpHandle(), 1, 0, over);

}

