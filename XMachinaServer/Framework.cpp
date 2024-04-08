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

#undef max 
#include <flatbuffers/flatbuffers.h>
#include "Protocol/FBProtocol_generated.h"


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
	mServer = Memory::Make_Shared<ServerNetwork>();
	mServer->SetMaxSessionCnt(1);
	mServer->SetSessionConstructorFunc(std::make_shared<GameSession>);
	mServer->Start(L"127.0.0.1", 7777);


	return true;
}

struct test {
private:
	int a{};
};

void Framework::Launch()
{

	LOG_MGR->SetColor(TextColor::BrightYellow);
	LOG_MGR->Cout("+-------------------------------\n");
	LOG_MGR->Cout("   X-MACHINA Server Framework   \n");
	LOG_MGR->Cout("-------------------------------+\n");
	LOG_MGR->SetColor(TextColor::Default);

	int ThreadNum = 4;
	for (INT32 i = 0; i < ThreadNum; ++i) {
		THREAD_MGR->RunThread("Network Dispatch " + std::to_string(i), [&]() {
	
			auto d  = TLS_MGR->Get_TlsInfoData();
			auto Tls_sendFactory = TLS_MGR->Get_TlsSendBufFactory();
			std::cout << d->id  << " " << Tls_sendFactory->strFactoryID << std::endl;
			while (true)
			{
				mServer->Dispatch_CompletedTasks_FromIOCP(0);
				//std::cout << TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory << std::endl;
			}
			});
	}

	THREAD_MGR->RunThread("Send Test", [&]() {
			while (true) {
				test Data;
				SPtr_SendPktBuf sendBuf = TLS_MGR->Get_TlsSendBufFactory()->SendBufFactory->CreateVarSendPacketBuf(sizeof(Data));
				//std::cout << sendBuf.get() << std::endl;
				mServer->Broadcast(sendBuf);
			}
		});
	


	THREAD_MGR->JoinAllThreads();
}
