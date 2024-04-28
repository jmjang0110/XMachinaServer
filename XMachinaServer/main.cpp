
#include "pch.h"
#include "Framework.h"
#include <crtdbg.h>


/// ====================================
/// 한국공학대학교 (前 한국산업기술대학교)
/// 게임이름 :	X-MACHINA
/// 팀 이름  :	민승재팀
/// 팀 구성  :	서버 - 장재문
///				클라 - 민동현, 백승호 
/// _____________________________________ 
/// XMachina - Server 
/// ------------------------------------
/// Server	 : IOCP  
/// DataBase : MySQL 
/// Client   : DirectX12
/// _____________________________________
/// 참고자료 모음 
/// IOCP MSDN : https://learn.microsoft.com/en-us/windows/win32/fileio/i-o-completion-ports
/// ---------------------------------------------------------------------------------------------+

BOOL WINAPI ConsoleHandler(DWORD dwCtrlType) {
    switch (dwCtrlType) {
    case CTRL_CLOSE_EVENT:
        // 여기서 추가 작업을 수행합니다.
        std::cout << "Console window closing..." << std::endl;
        // 여기에 메모리 해제 또는 자원 해제 등을 추가할 수 있습니다.
        // 예를 들어, 파일을 닫는다거나 소켓을 닫는 등의 작업을 수행할 수 있습니다.
        if (FRAMEWORK != nullptr) {
            FRAMEWORK->Destroy(); // 여기에 프레임워크의 종료 함수를 호출합니다.
        }
        break;
    default:
        break;
    }
    return FALSE; // FALSE를 반환하여 기존의 핸들러가 동작하도록 합니다.
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif // DEBUG

	// 프로세스 종료 핸들러를 등록합니다.
	if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
		std::cerr << "Error setting console control handler" << std::endl;
		return 1;
	}

	/* Server Framework Start */
	if (FRAMEWORK->Init(hInstance)) {
		FRAMEWORK->Launch();
		FRAMEWORK->Destroy();
	}
	else {
		FRAMEWORK->Destroy();
	}
	/* Server Framework End */


	::WSACleanup();

#ifdef _DEBUG

	//_CrtDumpMemoryLeaks();	
#endif // DEBUG

	return 0;
}
