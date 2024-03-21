
#include "pch.h"
#include "Framework.h"

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


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG

	/* Server Framework Start */
	if (FRAMEWORK->Init(hInstance)) {
		FRAMEWORK->Launch();
		FRAMEWORK->Destroy();
	}
	/* Server Framework End */

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();	
#endif // DEBUG

	return 0;
}