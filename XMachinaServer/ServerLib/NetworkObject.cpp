#include "pch.h"
#include "NetworkObject.h"

NetworkObject::NetworkObject()
{
	mSocketData.CreateSocket();

}

NetworkObject::~NetworkObject()
{
	mSocketData.Close();

}
