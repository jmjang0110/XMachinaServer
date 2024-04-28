
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
//
//constexpr int ThreadNum = 4;
//Lock::SRWLockGuard lock;
//int sum = 0;
//
//// 백만을 더하는 함수
//void addMillion( ) {
//    for (int i = 0; i < 1000'0000 / ThreadNum; ++i) {
//        lock.LockWrite(); // 쓰기 잠금을 획득합니다.
//        sum += 1; // 공유 변수를 증가시킵니다.
//        lock.UnloockWrite(); // 쓰기 잠금을 해제합니다.
//    }
//}
//void ReadData() {
//    for (int i = 0; i < 1000'0000; ++i) {
//        lock.LockRead(); // 쓰기 잠금을 획득합니다.
//        if (sum >= 1000'0000) {
//            lock.UnlockRead();
//            return;
//        }
//        sum += 100000;
//        std::cout << sum << std::endl;
//        lock.UnlockRead(); // 쓰기 잠금을 해제합니다.
//    }
//}
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPWSTR    lpCmdLine,
//    _In_ int       nCmdShow) 
//{
//    LOG_MGR->Init();
//
//    int sharedNum = 0; // 공유 변수를 정의합니다.
//
//    // 스레드를 저장할 벡터를 생성합니다.
//    std::vector<std::thread> threads;
//
//    auto start = std::chrono::high_resolution_clock::now(); // 시작 시간을 기록합니다.
//
//
//    // 4개의 스레드를 생성하고 백만을 더하는 함수를 실행합니다.
//    for (int i = 0; i < 4; ++i) {
//        threads.emplace_back(addMillion);
//    }
//    threads.emplace_back(ReadData);
//
//    // 모든 스레드의 실행이 종료될 때까지 기다립니다.
//    for (auto& thread : threads) {
//        thread.join();
//    }
//
//    auto end = std::chrono::high_resolution_clock::now(); // 종료 시간을 기록합니다.
//
//    // 실행 시간을 계산하여 출력합니다.
//    std::chrono::duration<double, std::milli> duration = end - start;
//    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
//
//
//    // 결과 출력
//    std::cout << "Result: " << sum << std::endl;
//
//    char a;
//    std::cin >> a;
//
//    return 0;
//}


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
