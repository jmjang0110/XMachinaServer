#include "pch.h"
#include "Framework.h"
#include "WindowUI/WindowUI.h"
#include "ServerLib/ThreadManager.h"
#include "ServerLib/NetworkManager.h"
#include "ServerLib/MemoryManager.h"

#include "ServerLib/ServerNetwork.h"
#include "ServerLib/Listener.h"
#include "ServerLib/SocketData.h"


#include "Contents/GameSession.h"




DEFINE_SINGLETON(Framework);
Framework::Framework()
{
}

Framework::~Framework()
{
	
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

	if (FALSE == TLS_MGR->Init()) {
		return false;
	}

	mServer = std::make_shared<ServerNetwork>();
	mServer->SetMaxSessionCnt(1);
	mServer->SetSessionConstructorFunc(std::make_shared<GameSession>);
	mServer->Start(L"127.0.0.1", 7777);


	return true;
}


void test2() {
	while (true) {
		//std::cout << TLS_MGR->Get_TlsInfoData()->threadName << " ID : " << TLS_MGR->Get_TlsInfoData()->id << std::endl;

	
	}
}


void Framework::Launch()
{
	LOG_MGR->SetColor(TextColor::BrightYellow);
	LOG_MGR->Cout("+-------------------------------\n");
	LOG_MGR->Cout("   X-MACHINA Server Framework   \n");
	LOG_MGR->Cout("-------------------------------+\n");
	LOG_MGR->SetColor(TextColor::Default);

	//THREAD_MGR->RunThread("Testtherad", [&]() {
	//		while (true) {
	//			std::cout << TLS_MGR->Get_TlsInfoData()->threadName 
	//				<< " ID : " << TLS_MGR->Get_TlsInfoData()->id << std::endl;
	//		}
	//	});


	for (INT32 i = 0; i < 6; ++i) {
		THREAD_MGR->RunThread("Network Dispatch " + std::to_string(i), [&]() {
	
			auto d = TLS_MGR->Get_TlsInfoData();
			auto d2 = TLS_MGR->Get_TlsSendBufFactory();
			std::cout << d->id  << " " << d2->strFactoryID << std::endl;

			while (true)
			{
				mServer->Dispatch_CompletedTasks_FromIOCP(10);
			}
			});
	}
	

	THREAD_MGR->JoinAllThreads();
}
