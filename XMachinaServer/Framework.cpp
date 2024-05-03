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
		return false;
	}

	/// +---------------------------------------
	///	XMachina Window Server UI : Dx12, Imgui  
	/// ---------------------------------------+
	if (FALSE == WINDOW_UI->Init(hInst, { 1366,768 })) {
		return false;
	}

	/// +----------------------------------------------------------------
	///	Network Manager : 2.2버젼 Winsock 초기화 및 비동기 함수 Lpfn 초기화
	/// ----------------------------------------------------------------+
	if (FALSE == NETWORK_MGR->Init()) {
		return false;
	}


	/// +------------------------------------
	///	TLS MGR : Thread Local Storage 관리 
	/// ------------------------------------+
	if (FALSE == TLS_MGR->Init()) {
		return false;

	}


	/// +------------------------
	///	MEMORY : Memory Pool 관리 
	/// ------------------------+
	if (FALSE == MEMORY->InitMemories()) {
		return false;
	}

	/// +-----------------------------------------
	///	GAME MANAGER : Game Room, Player ... 관리
	/// -----------------------------------------+
	GAME_MGR->Init();

	/// +-----------------------------------------
	///	SERVER NETWORK : IOCP Server Network 관리 
	/// -----------------------------------------+
	//mServer = std::make_shared<ServerNetwork>();
	mServer = Memory::Make_Shared<ServerNetwork>();
	mServer->SetMaxSessionCnt(5000); /* 최대 접속 세션 */
	mServer->SetSessionConstructorFunc(std::make_shared<GameSession>); /* GameSession으로 관리 */

	mServer->Start(L"127.0.0.1", 7777); /* Bind-Listen-AcceptEx.. */
	

	/// +-------------------------------------------------------------------
	///	SEND BUFFERS FACTORY : SendBuffer전용 메모리 풀 및 SendPktBuffer 생산
	/// -------------------------------------------------------------------+
	mSendFactory = std::make_shared<SendBuffersFactory>();
	mSendFactory->InitPacketMemoryPools();


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
