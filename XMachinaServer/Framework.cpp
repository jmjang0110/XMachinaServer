#include "pch.h"
#include "Framework.h"
#include "WindowUI/WindowUI.h"
#include "ServerLib/ThreadManager.h"
#include "ServerLib/NetworkManager.h"
#include "ServerLib/MemoryManager.h"
#include "Contents/GameManager.h"
#include "Contents/TimeManager.h"



#include "ServerLib/ServerNetwork.h"
#include "ServerLib/Listener.h"
#include "ServerLib/SocketData.h"


#include "Contents/GameSession.h"
#include "ServerLib/SendBuffersFactory.h"
#include "Protocol/FBsPacketFactory.h"
#include "Contents/DBController.h"
#include "Contents/ResourceManager.h"


#undef max 
#include <flatbuffers/flatbuffers.h>
#include "Protocol/FBProtocol_generated.h"


DEFINE_SINGLETON(Framework);
Framework::Framework()
{
}

Framework::~Framework()
{
	mServer->DecRef_AccpetOverlapped();
	mServer->DecRef_Listener();

	mServer      = nullptr;
	mSendFactory = nullptr;

	WINDOW_UI->Destroy();
	NETWORK_MGR->Destroy();
	MEMORY->Destroy();
	THREAD_MGR->Destroy();
	LOG_MGR->Destroy();
	DB_CONTROLLER->Destroy();


}

bool Framework::Init(HINSTANCE& hInst)
{




	/// +------------------------------------
	///	 Log Manager : Console I/O, File I/O 
	/// ------------------------------------+
	if (FALSE == LOG_MGR->Init()) {
		LOG_MGR->SetColor(TextColor::Red);
		LOG_MGR->Cout("[FAIL] LOG_MGR INIT\n");
		LOG_MGR->SetColor(TextColor::Default);

		return false;
	}

	LOG_MGR->Cout("[SUCCESS] LOG_MGR INIT\n");

	/// +------------------------------------------------------------------------------
	///						( X - 서버 성능 저하로 인해 폐지함. )
	/// _______________________________________________________________________________
	///	 XMachina Window Server UI : Dx12, Imgui - 서버 상태를 Imgui를 이용해 렌더링한다. 
	/// ------------------------------------------------------------------------------+
	if (FALSE == WINDOW_UI->Init(hInst, { 1366,768 })) {
		LOG_MGR->SetColor(TextColor::Red);
		LOG_MGR->Cout("[FAIL] WINDOW_UI INIT\n");
		LOG_MGR->SetColor(TextColor::Default);

		return false;
	}
	LOG_MGR->Cout("[SUCCESS] WINDOW_UI INIT\n");

	/// +----------------------------------------------------------------
	///	Network Manager : 2.2버젼 Winsock 초기화 및 비동기 함수 Lpfn 초기화
	/// ----------------------------------------------------------------+
	if (FALSE == NETWORK_MGR->Init()) {
		
		LOG_MGR->SetColor(TextColor::Red);
		LOG_MGR->Cout("[FAIL] NETWORK_MGR INIT\n");
		LOG_MGR->SetColor(TextColor::Default);

		return false;
	}
	LOG_MGR->Cout("[SUCCESS] NETWORK_MGR INIT\n");

	/// +------------------------------------
	///	TLS MGR : Thread Local Storage 관리 
	/// ------------------------------------+
	if (FALSE == TLS_MGR->Init()) {

		LOG_MGR->SetColor(TextColor::Red);
		LOG_MGR->Cout("[FAIL] TLS_MGR INIT\n");
		LOG_MGR->SetColor(TextColor::Default);

		return false;

	}

	LOG_MGR->Cout("[SUCCESS] TLS_MGR INIT\n");


	/// +------------------------
	///	MEMORY : Memory Pool 관리 
	/// ------------------------+
	if (FALSE == MEMORY->InitMemories()) {

		LOG_MGR->SetColor(TextColor::Red);
		LOG_MGR->Cout("[FAIL] MEMORY INIT\n");
		LOG_MGR->SetColor(TextColor::Default);

		return false;
	}

	LOG_MGR->Cout("[SUCCESS] MEMORY INIT\n");


	/// +-----------------------------------------
	///	GAME MANAGER : Game Room, Player ... 관리
	/// -----------------------------------------+
	RESOURCE_MGR->Init();	// Scene, Models ... ( Game Resources ) Load 
	GAME_MGR->Init();		// Room - PlayerController, SectorController, NPCController ( Init )


	LOG_MGR->Cout("[SUCCESS] GAME_MGR INIT\n");

	std::string ipAddresses = GetLocalIPv4Address();
	if (!ipAddresses.empty()) {
		mServerIP = string_to_wstring(ipAddresses);
		LOG_MGR->WCout(L"wifi IPv4 Address : ", mServerIP, '\n');
	}
	else {
		mServerIP = L"127.0.0.1";
		LOG_MGR->SetColor(TextColor::BrightRed);
		LOG_MGR->Cout("No Wi-Fi IPv4 Address found. \n ServerIP LoopBack Setting : 127.0.0.1 \n");
		LOG_MGR->SetColor(TextColor::Default);

		LOG_MGR->Cout("Server IP 입력 : ");
		std::wcin >> mServerIP;
		LOG_MGR->WCout(L"IPv4 Address : ", mServerIP, '\n');
	}

	/// +-----------------------------------------
	///	SERVER NETWORK : IOCP Server Network 관리 
	/// -----------------------------------------+
	LOG_MGR->Cout("[ING...] ( PLEASE WAIT ) ServerNetwork INIT \n");
	{
		mServer = Memory::Make_Shared<ServerNetwork>();
		mServer->SetMaxSessionCnt(MAX_SESSION_NUM); /* 최대 접속 세션 */
		mServer->SetSessionConstructorFunc(std::make_shared<GameSession>); /* GameSession으로 관리 */

		//mServerIP = L"127.0.0.1";
		mServer->Start(mServerIP, 7777); /* Bind-Listen-AcceptEx.. */
	}	
	LOG_MGR->Cout("[SUCCESS] ServerNetwork INIT\n");


	DB_CONTROLLER->Init();
	DB_CONTROLLER->ConnectToDatabase(L"X_Machina_DB", L"jmjang016", L"1234");

	/// +-------------------------------------------------------------------
	///	SEND BUFFERS FACTORY : SendBuffer전용 메모리 풀 및 SendPktBuffer 생산
	/// -------------------------------------------------------------------+
	LOG_MGR->Cout("[ING...] ( PLEASE WAIT ) SendBuffersFactory INIT\n");
	{
		mSendFactory = std::make_shared<SendBuffersFactory>();
		mSendFactory->InitPacketMemoryPools();
	}
	LOG_MGR->Cout("[SUCCESS] SendBuffersFactory INIT\n");





	return true;
}


void Framework::Launch()
{


#ifdef  CONNECT_WITH_TEST_CLIENT
	LOG_MGR->SetColor(TextColor::BrightMagenta);
	LOG_MGR->Cout("--------------------------------\n");
	LOG_MGR->Cout("▶ CONNECT WITH TEST CLIENT     \n");
	LOG_MGR->Cout("--------------------------------\n");
#else 
	LOG_MGR->SetColor(TextColor::BrightCyan);
	LOG_MGR->Cout("---------------------------------------------------------------\n");
	LOG_MGR->Cout("▶ CONNECT WITH X-MACHINA CLIENT");
	LOG_MGR->WCout(" - SERVER IP : ", mServerIP, '\n');
	LOG_MGR->Cout("---------------------------------------------------------------\n");
#endif //  CONNECT_WITH_TEST_CLIENT

	LOG_MGR->SetColor(TextColor::BrightYellow);
	LOG_MGR->Cout("+---------------------------------\n");
	LOG_MGR->Cout("♣  X-MACHINA Server Framework  ♣ \n");
	LOG_MGR->Cout("---------------------------------+\n");
	LOG_MGR->SetColor(TextColor::Default);

	// Seed the random number generator
	std::srand(static_cast<unsigned>(std::time(0)));

	int CoreNum = 4; /* Network Threads */;
	std::cout << "Core : " << CoreNum << std::endl;
	std::atomic<bool> stop(false);

	/// +---------------------- IOCP WORKER THREAD : 4 (worker Thread ---------------------------+
	/* 4 : IOCP Thread  1 : Timer Thread*/
	for (INT32 i = 1; i <= CoreNum ; ++i) { 
	
		THREAD_MGR->RunThread("Worker Threads : 4 (CoreNum - 1(Timer))" + std::to_string(i), [&]() {

			UINT32 msTimeOut = 0;
			double time = 0;
			double prev = 0;
			while (!stop.load())
			{
				TLS_MGR->Get_TlsInfoData()->TimeMgr.Tick(240.f);
				mServer->WorkerThread(msTimeOut);

				time += TLS_MGR->Get_TlsInfoData()->TimeMgr.GetTimeElapsed();

		/*		if ((int)time != (int)prev) {
					LOG_MGR->Cout("[", TLS_MGR->Get_TlsInfoData()->id,"] : ", time, "\n");
					prev = time;
				}*/
			}

			});
	}

	/// +-------------------------	TIMER THREAD : 1 (main) ------------------------------+
	TIME_MGR->Launch();

#ifdef  CONNECT_WITH_TEST_CLIENT
	THREAD_MGR->RunThread("Send Test", [&]() {
		while (false) {
			SPtr_SendPktBuf SPkt = FBS_FACTORY->SPkt_Chat(0, "test Chat");
			if (SPkt) {
				//mServer->Broadcast(SPkt);
				//std::this_thread::sleep_for(std::chrono::milliseconds(100));

			}
		}
		});
#endif //  CONNECT_WITH_TEST_CLIENT


	THREAD_MGR->JoinAllThreads();
}

long long Framework::GetCurrentTimeMilliseconds()
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

std::string Framework::GetLocalIPv4Address()

{
	std::string ipAddress;

	// 초기화
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 네트워크 인터페이스 정보 가져오기
	ULONG bufferSize = 0;
	if (GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &bufferSize) == ERROR_BUFFER_OVERFLOW) {
		std::vector<char> buffer(bufferSize);
		PIP_ADAPTER_ADDRESSES addressesBuffer = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(&buffer[0]);

		if (GetAdaptersAddresses(AF_INET, 0, nullptr, addressesBuffer, &bufferSize) == NO_ERROR) {
			for (PIP_ADAPTER_ADDRESSES adapter = addressesBuffer; adapter != nullptr; adapter = adapter->Next) {
				if (adapter->IfType == IF_TYPE_IEEE80211 && adapter->OperStatus == IfOperStatusUp) {
					IP_ADAPTER_UNICAST_ADDRESS* unicast = adapter->FirstUnicastAddress;
					if (unicast != nullptr) {
						sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(unicast->Address.lpSockaddr);
						char ipBuffer[INET_ADDRSTRLEN];
						inet_ntop(AF_INET, &(addr->sin_addr), ipBuffer, INET_ADDRSTRLEN);
						ipAddress = ipBuffer;
						break; // 첫 번째 IP만 필요하므로 루프 종료
					}
				}
			}
		}
	}

	WSACleanup();
	return ipAddress;
}

std::wstring Framework::string_to_wstring(const std::string& str)
{
	std::wstring wstr(str.begin(), str.end());
	return wstr;
}
