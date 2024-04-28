
#include "pch.h"
#include "Framework.h"
#include <crtdbg.h>


/// ====================================
/// �ѱ����д��б� (�� �ѱ����������б�)
/// �����̸� :	X-MACHINA
/// �� �̸�  :	�ν�����
/// �� ����  :	���� - ���繮
///				Ŭ�� - �ε���, ���ȣ 
/// _____________________________________ 
/// XMachina - Server 
/// ------------------------------------
/// Server	 : IOCP  
/// DataBase : MySQL 
/// Client   : DirectX12
/// _____________________________________
/// �����ڷ� ���� 
/// IOCP MSDN : https://learn.microsoft.com/en-us/windows/win32/fileio/i-o-completion-ports
/// ---------------------------------------------------------------------------------------------+

BOOL WINAPI ConsoleHandler(DWORD dwCtrlType) {
    switch (dwCtrlType) {
    case CTRL_CLOSE_EVENT:
        // ���⼭ �߰� �۾��� �����մϴ�.
        std::cout << "Console window closing..." << std::endl;
        // ���⿡ �޸� ���� �Ǵ� �ڿ� ���� ���� �߰��� �� �ֽ��ϴ�.
        // ���� ���, ������ �ݴ´ٰų� ������ �ݴ� ���� �۾��� ������ �� �ֽ��ϴ�.
        if (FRAMEWORK != nullptr) {
            FRAMEWORK->Destroy(); // ���⿡ �����ӿ�ũ�� ���� �Լ��� ȣ���մϴ�.
        }
        break;
    default:
        break;
    }
    return FALSE; // FALSE�� ��ȯ�Ͽ� ������ �ڵ鷯�� �����ϵ��� �մϴ�.
}
//
//constexpr int ThreadNum = 4;
//Lock::SRWLockGuard lock;
//int sum = 0;
//
//// �鸸�� ���ϴ� �Լ�
//void addMillion( ) {
//    for (int i = 0; i < 1000'0000 / ThreadNum; ++i) {
//        lock.LockWrite(); // ���� ����� ȹ���մϴ�.
//        sum += 1; // ���� ������ ������ŵ�ϴ�.
//        lock.UnloockWrite(); // ���� ����� �����մϴ�.
//    }
//}
//void ReadData() {
//    for (int i = 0; i < 1000'0000; ++i) {
//        lock.LockRead(); // ���� ����� ȹ���մϴ�.
//        if (sum >= 1000'0000) {
//            lock.UnlockRead();
//            return;
//        }
//        sum += 100000;
//        std::cout << sum << std::endl;
//        lock.UnlockRead(); // ���� ����� �����մϴ�.
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
//    int sharedNum = 0; // ���� ������ �����մϴ�.
//
//    // �����带 ������ ���͸� �����մϴ�.
//    std::vector<std::thread> threads;
//
//    auto start = std::chrono::high_resolution_clock::now(); // ���� �ð��� ����մϴ�.
//
//
//    // 4���� �����带 �����ϰ� �鸸�� ���ϴ� �Լ��� �����մϴ�.
//    for (int i = 0; i < 4; ++i) {
//        threads.emplace_back(addMillion);
//    }
//    threads.emplace_back(ReadData);
//
//    // ��� �������� ������ ����� ������ ��ٸ��ϴ�.
//    for (auto& thread : threads) {
//        thread.join();
//    }
//
//    auto end = std::chrono::high_resolution_clock::now(); // ���� �ð��� ����մϴ�.
//
//    // ���� �ð��� ����Ͽ� ����մϴ�.
//    std::chrono::duration<double, std::milli> duration = end - start;
//    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
//
//
//    // ��� ���
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

	// ���μ��� ���� �ڵ鷯�� ����մϴ�.
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
