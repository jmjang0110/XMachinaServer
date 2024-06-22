#pragma once
#include "Client.h"



class Network
{
	DECLARE_SINGLETON(Network);

public:
	static int Global_Delay; // ms단위로 1000ms 가 넘으면 클라이언트 증가를 종료한다. 

private:
	HANDLE m_IOCP;
	high_resolution_clock::time_point m_Last_Connect_Time;

	std::array<int, MAX_CLIENTS> client_map;
	std::array<Client, MAX_CLIENTS> g_clients;

	std::atomic_int m_Num_connections;
	std::atomic_int m_Client_to_close;
	std::atomic_int m_Active_clients;

	std::vector<std::thread*> m_WorkerThreads;
	std::thread m_Test_Thread;

public:
	/// +--------------------------------------
	///		Network 
	/// --------------------------------------+
	void Error_Display(const char* msg, int errNO);
	void Disconnect_Client(int clientIdx);

	void Init_Netowkr();
	void ShutDown_Network();
	void Do_Network();



	/// +--------------------------------------
	///		Packet 
	/// --------------------------------------+
	void SendPacket(int clientIdx, void* packet);
	void ProcessPacket(int clientIdx, unsigned char packet[]);


	/// +--------------------------------------
	///	 Thread
	/// --------------------------------------+
	void Worker_Thread();
	void Test_Thread();


	/// +--------------------------------------
	///	  Server Stress Test 
	/// --------------------------------------+
	void Adjust_Number_Of_Client();


};

