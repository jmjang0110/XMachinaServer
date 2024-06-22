

#include "pch.h"
#include "Framework.h"

/// +----------------------------------------------------------------------------------------------------------------------------------------------
/// * 정내훈 교수님 게임서버프로그래밍 02반 개발 일지 *  
/// ===============================================================================================================================================
/// 2024-03-07 (목) - [과제1] 게임 프레임워크 제작, Chess판, 체스 말 하나를 상하좌우로 움직일 수 있게 한다. 
/// ----------------------------------------------------------------------------------------------------------------------------------------------+

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif // _DEBUG

	FILE* mConsoleStream{};	/// Console 

	AllocConsole();
	freopen_s(&mConsoleStream, "CONOUT$", "w", stdout); // 표준 출력을 콘솔로 리디렉션
	freopen_s(&mConsoleStream, "CONIN$", "r", stdin);	// 표준 입력을 콘솔로 리디렉션

	std::cout << "+---------------------------\n";
	std::cout << "2018180035 장재문 - Client   \n";
	std::cout << "----------------------------+\n";

#pragma region Client Framework
	if (FRAMEWORK->Init(hInstance, RESOLUTION{ 700,550 })) {

		FRAMEWORK->Loop();
		FRAMEWORK->Destroy();
	}
#pragma endregion
	FreeConsole();
	fclose(mConsoleStream);

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
#endif // _DEBUG

}


