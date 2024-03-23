
#include "pch.h"
#include "Framework.h"

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

	::WSACleanup();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();	
#endif // DEBUG

	return 0;
}