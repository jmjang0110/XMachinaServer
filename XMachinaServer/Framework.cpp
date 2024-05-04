#include "pch.h"
#include "Framework.h"
#include "WindowUI/WindowUI.h"
#include "ServerLib/ThreadManager.h"
#include "ServerLib/NetworkManager.h"
#include "ServerLib/MemoryManager.h"
#include "Contents/GameManager.h"


#include "ServerLib/ServerNetwork.h"
#include "ServerLib/Listener.h"
#include "ServerLib/SocketData.h"


#include "Contents/GameSession.h"
#include "ServerLib/SendBuffersFactory.h"

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
	/* Window UI Destroy */
	WINDOW_UI->Destroy();

	/* Log Manager Destroy */
	LOG_MGR->Destroy();

	/* Thread Manager Destroy --> (TLS) threadLocalStorageManager Destroy */
	THREAD_MGR->Destroy();

	/* Network Manager Destroy */
	NETWORK_MGR->Destroy();

	/* Memory Manager Destroy */
	MEMORY->Destroy();


}

bool Framework::Init(HINSTANCE& hInst)
{

	/// +------------------------------------
	///	 Log Manager : Console I/O, File I/O 
	/// ------------------------------------+
	if (FALSE == LOG_MGR->Init()) {
		LOG_MGR->Cout("LOG_MGR INIT... FAIL\n");

		return false;
	}

	LOG_MGR->Cout("LOG_MGR INIT... SUCCESS\n");

	/// +---------------------------------------
	///	XMachina Window Server UI : Dx12, Imgui  
	/// ---------------------------------------+
	if (FALSE == WINDOW_UI->Init(hInst, { 1366,768 })) {
		LOG_MGR->Cout("WINDOW_UI INIT... FAIL\n");

		return false;
	}

	LOG_MGR->Cout("WINDOW_UI INIT... SUCCESS\n");

	/// +----------------------------------------------------------------
	///	Network Manager : 2.2버젼 Winsock 초기화 및 비동기 함수 Lpfn 초기화
	/// ----------------------------------------------------------------+
	if (FALSE == NETWORK_MGR->Init()) {
		LOG_MGR->Cout("NETWORK_MGR INIT... FAIL\n");

		return false;
	}

	LOG_MGR->Cout("NETWORK_MGR INIT... SUCCESS\n");

	/// +------------------------------------
	///	TLS MGR : Thread Local Storage 관리 
	/// ------------------------------------+
	if (FALSE == TLS_MGR->Init()) {
		LOG_MGR->Cout("TLS_MGR INIT... FAIL\n");

		return false;

	}

	LOG_MGR->Cout("TLS_MGR INIT... SUCCESS\n");


	/// +------------------------
	///	MEMORY : Memory Pool 관리 
	/// ------------------------+
	if (FALSE == MEMORY->InitMemories()) {
		LOG_MGR->Cout("MEMORY INIT... SUCCESS\n");

		return false;
	}

	LOG_MGR->Cout("MEMORY INIT... SUCCESS\n");

	/// +-----------------------------------------
	///	GAME MANAGER : Game Room, Player ... 관리
	/// -----------------------------------------+
	GAME_MGR->Init();

	LOG_MGR->Cout("GAME_MGR INIT... SUCCESS\n");

	/// +-----------------------------------------
	///	SERVER NETWORK : IOCP Server Network 관리 
	/// -----------------------------------------+
	LOG_MGR->Cout("ServerNetwork INIT... ING... PLEASE WAIT \n");

	//mServer = std::make_shared<ServerNetwork>();
	mServer = Memory::Make_Shared<ServerNetwork>();
	mServer->SetMaxSessionCnt(5000); /* 최대 접속 세션 */
	mServer->SetSessionConstructorFunc(std::make_shared<GameSession>); /* GameSession으로 관리 */

	mServer->Start(L"127.0.0.1", 7777); /* Bind-Listen-AcceptEx.. */
	
	LOG_MGR->Cout("ServerNetwork INIT... SUCCESS\n");

	/// +-------------------------------------------------------------------
	///	SEND BUFFERS FACTORY : SendBuffer전용 메모리 풀 및 SendPktBuffer 생산
	/// -------------------------------------------------------------------+
	LOG_MGR->Cout("SendBuffersFactory INIT... ING... PLEASE WAIT \n");

	mSendFactory = std::make_shared<SendBuffersFactory>();
	mSendFactory->InitPacketMemoryPools();

	LOG_MGR->Cout("SendBuffersFactory INIT... SUCCESS\n");


	return true;
}

void Framework::Launch()
{

	LOG_MGR->SetColor(TextColor::BrightYellow);
	LOG_MGR->Cout("+-------------------------------\n");
	LOG_MGR->Cout("   X-MACHINA Server Framework   \n");
	LOG_MGR->Cout("-------------------------------+\n");
	LOG_MGR->SetColor(TextColor::Default);

	int CoreNum = 4;
	std::cout << "Core : " << CoreNum << std::endl;
	std::atomic<bool> stop(false);

	for (INT32 i = 0; i < CoreNum; ++i) {
		THREAD_MGR->RunThread("Network Dispatch " + std::to_string(i), [&]() {
	
			auto d               = TLS_MGR->Get_TlsInfoData();
			auto Tls_sendFactory = TLS_MGR->Get_TlsSendBufFactory();
			//std::cout << d->id  << " " << Tls_sendFactory->strFactoryID << std::endl;
			while (!stop.load())
			{
				mServer->Dispatch_CompletedTasks_FromIOCP(0);
			}
			});
	}

	std::cout << "Q : stop\n";
	char key;
	std::cin >> key;

	// If 'q' is pressed, stop the threads
	if (key == 'q') {
		stop = true;
	}

	/*THREAD_MGR->RunThread("Send Test", [&]() {
			while (true) {
				SPtr_SendPktBuf SPkt = mSendFactory->SPkt_Chat(0, "test Chat");
				if (SPkt) {
					mServer->Broadcast(SPkt);
				}
			}
		});*/
	


	THREAD_MGR->JoinAllThreads();
}
