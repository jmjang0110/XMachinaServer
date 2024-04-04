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
#include "ServerLib/SendBuffersFactory.h"



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
	///	Network Manager : 2.2���� Winsock �ʱ�ȭ �� �񵿱� �Լ� Lpfn �ʱ�ȭ
	/// ----------------------------------------------------------------+
	if (FALSE == NETWORK_MGR->Init()) {
		return false;
	}

	if (FALSE == TLS_MGR->Init()) {
		return false;
	}

	if (FALSE == MEMORY->InitMemories()) {
		return false;
	}

	//mServer = std::make_shared<ServerNetwork>();
	mServer = MemoryManager::Make_Shared<ServerNetwork>();
	mServer->SetMaxSessionCnt(1);
	mServer->SetSessionConstructorFunc(std::make_shared<GameSession>);
	mServer->Start(L"127.0.0.1", 7777);


	return true;
}

class Test {
private:
	int a; 
	int b;
	int c;
public:
	Test() {};
	Test(int _a, int _b, int _c) { a = _a; b = _b; c = _c; }
	~Test()
	{
		;
	}
};

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


	for (INT32 i = 0; i < 4; ++i) {
		THREAD_MGR->RunThread("Network Dispatch " + std::to_string(i), [&]() {
	
			auto d  = TLS_MGR->Get_TlsInfoData();
			auto d2 = TLS_MGR->Get_TlsSendBufFactory();
			std::cout << d->id  << " " << d2->strFactoryID << std::endl;
			//std::shared_ptr<Test> mtest = MemoryManager::Make_Shared<Test>(1, 2, 3);
			while (true)
			{
				mServer->Dispatch_CompletedTasks_FromIOCP(0);

			}
			});
	}
	

	THREAD_MGR->JoinAllThreads();
}
