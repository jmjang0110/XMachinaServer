#include "pch.h"
#include "Framework.h"
#include "WindowUI/WindowUI.h"
#include "ServerLib/ThreadManager.h"
#include "ServerLib/NetworkManager.h"



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

	/// +---------------------------------------
	///	Network Manager : 
	/// ---------------------------------------+
	if (FALSE == NETWORK_MGR->Init()) {

	}

	return true;
}

void test() {
	while (true) {
		std::cout << TLS_MGR->Get_TlsInfoData()->threadName << " ID : " << TLS_MGR->Get_TlsInfoData()->id << std::endl;
	}
}

void test2() {
	while (true) {
		std::cout << TLS_MGR->Get_TlsInfoData()->threadName << " ID : " << TLS_MGR->Get_TlsInfoData()->id << std::endl;

	}
}
void Framework::Launch()
{
	LOG_MGR->SetColor(TextColor::BrightGreen);
	LOG_MGR->Cout("+-------------------------------\n");
	LOG_MGR->Cout("   X-MACHINA Server Framework   \n");
	LOG_MGR->Cout("-------------------------------+\n");
	LOG_MGR->SetColor(TextColor::Default);

	THREAD_MGR->RunThread("Testtherad", test);
	THREAD_MGR->RunThread("Testtherad2", test2);


	THREAD_MGR->JoinAllThreads();

}
