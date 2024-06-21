#include "pch.h"
#include "Framework.h"

DEFINE_SINGLETON(Framework);
int Framework::Global_Delay = 0;



Framework::Framework()
{
}

Framework::~Framework()
{
}

void Framework::Error_Display(const char* msg, int errNO)
{
}

void Framework::Disconnect_Client(int clientIdx)
{
}

void Framework::Init_Netowkr()
{
}

void Framework::ShutDown_Network()
{
}

void Framework::Do_Network()
{
}

void Framework::SendPacket(int clientIdx, void* packet)
{
}

void Framework::ProcessPacket(int clientIdx, unsigned char packet[])
{
}

void Framework::Worker_Thread()
{
}

void Framework::Test_Thread()
{
}

void Framework::Adjust_Number_Of_Client()
{
}
