#include "pch.h"
#include "ServerNetwork.h"
#include "NetworkInterface.h"
#include "Listener.h"


ServerNetwork::ServerNetwork(std::wstring ip, UINT16 portNum)
	: NetworkInterface(ip, portNum)
{
	mListener = std::make_shared<Listener>();

}

ServerNetwork::~ServerNetwork()
{

}

bool ServerNetwork::Start()
{

	mListener->Start(shared_from_this());



	return true;
}

void ServerNetwork::Close()
{
	NetworkInterface::Close();
}
