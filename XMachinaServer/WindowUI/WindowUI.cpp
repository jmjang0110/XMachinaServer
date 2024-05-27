#include "pch.h"
#include "WindowUI.h"

#include "../Contents/LogManager.h"

DEFINE_SINGLETON(WindowUI);
HINSTANCE	WindowUI::mhInst{};
HWND		WindowUI::mHwnd{};
HACCEL		WindowUI::mAccelTable{};
MSG		    WindowUI::mMsg{};

WindowUI::WindowUI()
{
}

WindowUI::~WindowUI()
{
    LOG_MGR->Cout("~WindowUI\n");

}

bool WindowUI::Init(HINSTANCE& hInstance, WindowResolution Res)
{
    mRes = Res;
    mhInst = hInstance;
    return true;

}

void WindowUI::Launch()
{
    LOG_MGR->SetColor(TextColor::BrightCyan);
    LOG_MGR->Cout("WindowUI Launch!");
    LOG_MGR->SetColor(TextColor::Default);

    if (WindowUI::CreateWindowUI()) {
        WindowUI::Loop();
    }

}

int WindowUI::Loop()
{
    /// +--------------------
    /// Server UI Main Loop
    /// --------------------+ 
    mAccelTable = LoadAccelerators(mhInst, MAKEINTRESOURCE(IDI_XMACHINASERVER));

    while (mIsRun)
    {
        if (PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE))
        {
            if (mMsg.message == WM_QUIT) break;
            if (!TranslateAccelerator(mMsg.hwnd, mAccelTable, &mMsg))
            {
                TranslateMessage(&mMsg);
                DispatchMessage(&mMsg);
                continue; // ImGui 

            }
        }
        else
        {

            const int bufferSize = 100; // 버퍼 크기 정의
            char buffer[bufferSize];    // 입력을 저장할 배열
            std::cout << "종료하려면 아무 키나 입력하세요 : ";
            std::cin.getline(buffer, bufferSize); // 사용자로부터 문자열 입력 받음
        }
    }

    ::DestroyWindow(mHwnd);
    ::UnregisterClass(L"XMachina IOCP Server", mhInst);

    return (int)mMsg.wParam;
}

void WindowUI::Logic()
{

}



bool WindowUI::CreateWindowUI()
{
    WNDCLASSEXW wcex{};
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WindowUI::WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = mhInst;
    wcex.hIcon         = LoadIcon(mhInst, MAKEINTRESOURCE(IDI_XMACHINASERVER));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = nullptr;   // MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName = L"XMachina IOCP Server";
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    ::RegisterClassExW(&wcex);

    mHwnd = CreateWindowW(L"XMachina IOCP Server", L"XMachina IOCP Server"
                        , WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0
                        , mRes.Width, mRes.Height
                        , nullptr, nullptr, mhInst, nullptr);

    ::ShowWindow(mHwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(mHwnd);


    return true;
}

LRESULT WindowUI::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

