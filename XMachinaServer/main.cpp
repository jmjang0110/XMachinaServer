
#include "pch.h"
#include "Framework.h"

/// ====================================
/// ÇÑ±¹°øÇÐ´ëÇÐ±³ 
/// ÆÀ ÀÌ¸§ :	¹Î½ÂÀçÆÀ
/// ÆÀ ±¸¼º :	¼­¹ö - ÀåÀç¹®
///				Å¬¶ó - ¹Îµ¿Çö, ¹é½ÂÈ£ 
/// _____________________________________ 
/// XMachina - Server 
/// ------------------------------------
/// Server	 : IOCP  
/// DataBase : MySQL 
/// ====================================

/// +---------------------------------------------------------------------------------------------
/// Âü°íÀÚ·á ¸ðÀ½ 
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