#include "pch.h"
#include "NetworkObject.h"

NetworkObject::NetworkObject()
{

}

NetworkObject::~NetworkObject()
{
	mSocketData.Close();

}

void NetworkObject::SetID(UINT32 id)
{
	ID = id;
}
