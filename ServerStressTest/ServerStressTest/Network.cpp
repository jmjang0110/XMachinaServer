#include "pch.h"
#include "Network.h"

DEFINE_SINGLETON(Network);
int Network::Global_Delay = 0;




void Network::Error_Display(const char* msg, int errNO)
{
}

void Network::Disconnect_Client(int clientIdx)
{
}

void Network::Init_Netowkr()
{
}

void Network::ShutDown_Network()
{
}

void Network::Do_Network()
{
}

void Network::SendPacket(int clientIdx, void* packet)
{
}

void Network::ProcessPacket(int clientIdx, unsigned char packet[])
{
}

void Network::Worker_Thread()
{
}

void Network::Test_Thread()
{
}

void Network::Adjust_Number_Of_Client()
{
}
