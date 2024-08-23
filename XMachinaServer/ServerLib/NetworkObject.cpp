#include "pch.h"
#include "NetworkObject.h"

NetworkObject::NetworkObject()
{

}

NetworkObject::NetworkObject(UINT32 id, std::string name)
	: mID(id), mName(name)
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

