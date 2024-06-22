

#include "pch.h"
#include "Framework.h"

/// +----------------------------------------------------------------------------------------------------------------------------------------------
/// * ������ ������ ���Ӽ������α׷��� 02�� ���� ���� *  
/// ===============================================================================================================================================
/// 2024-03-07 (��) - [����1] ���� �����ӿ�ũ ����, Chess��, ü�� �� �ϳ��� �����¿�� ������ �� �ְ� �Ѵ�. 
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
	freopen_s(&mConsoleStream, "CONOUT$", "w", stdout); // ǥ�� ����� �ַܼ� ���𷺼�
	freopen_s(&mConsoleStream, "CONIN$", "r", stdin);	// ǥ�� �Է��� �ַܼ� ���𷺼�

	std::cout << "+---------------------------\n";
	std::cout << "2018180035 ���繮 - Client   \n";
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


