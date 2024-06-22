#include "pch.h"
#include "Framework.h"
#include "Win32RenderManager.h"
#include "Scene.h"
#include "Key.h"
#include "ThreadManager.h"

#include "PacketFactory.h"
#include "TransformScript.h"


DEFINE_SINGLETON(Framework);

BYTE Framework::mIsRun = 0x01;

Framework::Framework()
{
}

Framework::~Framework()
{
    SAFE_DELETE(mGameScene);
    RENDER_MGR->Destroy();
    KEY_MGR->Destroy();
    SAFE_DELETE(mNetService);

}

int Framework::Loop()
{

    {
        /* Player 생성 */
        Object* Player = new Object("WhitePiece_Pawn");
        Player->setID(1234);
        Player->SetImage(L"Resource\\test.png", VECTOR2(0, 0), VECTOR2(50, 50)); // 512 512 

        Player->setPos(VECTOR2(20, 5));
        Player->SetScale(VECTOR2(60, 60));

        TransformScript* Scr = new TransformScript(Player);
        VECTOR2 MoveAmount = VECTOR2(60, 60);
        Scr->SetMoveAmount(MoveAmount);

        Scene* scene = FRAMEWORK->GetGameScene();
        if (scene->GetPlayer() == nullptr)
            scene->RegisterPlayer(Player);
        scene->RegisterObject(Player);

        Launch(ThreadName::network);
    }

    int msg = Launch_main();

    THREAD_MGR->Join();
    return msg;
}

void Framework::Logic()
{
    if (mGameScene) {
        ProcessEvent();

        KEY_MGR->update();              /// KEY 입력 감지 
        SendServerPacket();

        mGameScene->Update();           /// Game Scene 업데이트
        RENDER_MGR->Render_Prepare();   /// Game Rendering Prepare
        mGameScene->Render();           /// Game Object 렌더링
        RENDER_MGR->Render_Present();   /// Game Redering Present 


    }

}
void Framework::SendServerPacket()
{
    const int ID = FRAMEWORK->GetGameScene()->GetPlayer()->GetID();

    if (KEY_TAP(KEY::LEFT)) {
        SOCKET Serversock = FRAMEWORK->GetNerworkService()->GetSocket();
        UINT16 id = ID;
        UINT16 key = static_cast<UINT16>(KEY::LEFT);

        PacketFactory::Send_ClientPacket_KeyInput(Serversock, id, key);
    }
    if (KEY_TAP(KEY::RIGHT)) {
        SOCKET Serversock = FRAMEWORK->GetNerworkService()->GetSocket();
        UINT16 id = ID;
        UINT16 key = static_cast<UINT16>(KEY::RIGHT);

        PacketFactory::Send_ClientPacket_KeyInput(Serversock, id, key);
    }
    if (KEY_TAP(KEY::UP)) {
        SOCKET Serversock = FRAMEWORK->GetNerworkService()->GetSocket();
        UINT16 id = ID;
        UINT16 key = static_cast<UINT16>(KEY::UP);

        PacketFactory::Send_ClientPacket_KeyInput(Serversock, id, key);
    }
    if (KEY_TAP(KEY::DOWN)) {
        SOCKET Serversock = FRAMEWORK->GetNerworkService()->GetSocket();
        UINT16 id = ID;
        UINT16 key = static_cast<UINT16>(KEY::DOWN);

        PacketFactory::Send_ClientPacket_KeyInput(Serversock, id, key);
    }
}



void Framework::ProcessEvent()
{
    mNewObjEventLock.lock();

    while (mNewObjectEvents.empty() == false) {

        Object* obj = mNewObjectEvents.front();
        mNewObjectEvents.pop();
        mGameScene->RegisterObject(obj);

    }

    mNewObjEventLock.unlock();
}

void Framework::PushNewObjEvent(Object* obj)
{
    mNewObjEventLock.lock();

    mNewObjectEvents.push(obj);

    mNewObjEventLock.unlock();
}

bool Framework::Init(HINSTANCE& hInstance, RESOLUTION Res)
{
    mhInst = hInstance;
    mResolution = Res;
    Create();


    KEY_MGR->init();
    RENDER_MGR->Init(Res, mhWnd);
    mGameScene = new Scene;
    mGameScene->Init();

    mNetService = new NetworkService;
    mNetService->Init(); // connect 

    return true;
}


bool Framework::Create()
{
    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = mhInst;
    wcex.hIcon = LoadIcon(mhInst, MAKEINTRESOURCE(IDI_CHESS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName = L"2018180035 게임공학과 장재문 Client";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    mhWnd = CreateWindowW(L"2018180035 게임공학과 장재문 Client", L"2018180035 게임공학과 장재문 Client", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, mResolution.Width, mResolution.Height, nullptr, nullptr, mhInst, nullptr);


    ShowWindow(mhWnd, SW_SHOWDEFAULT);
    UpdateWindow(mhWnd);

    return true;
}


void Framework::ChangeWindowSize(HWND _hwnd, const RESOLUTION& res)
{
    RECT rt = { 0, 0, (int)res.Width, (int)res.Height };

    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

    RECT rect_desktop = { 0, };
    HWND hWnd_desktop = GetDesktopWindow();
    GetWindowRect(hWnd_desktop, &rect_desktop);

    RECT rect_console = { 0, };
    HWND hWnd_console = GetConsoleWindow();
    GetWindowRect(hWnd_console, &rect_console);

    // 중앙에 화면 출력
    SetWindowPos(_hwnd
        , nullptr
        , (rect_desktop.left + rect_desktop.right - rect_console.right + rect_console.left) / 2 - (rt.right - rt.left) / 2
        , (rect_desktop.top + rect_desktop.bottom - rect_console.bottom + rect_console.top) / 2 - (rt.bottom - rt.top) / 2
        , rt.right - rt.left
        , rt.bottom - rt.top
        , 0);
}

LRESULT Framework::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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


// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK Framework::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


const HINSTANCE& Framework::GetHINSTANCE()
{
    return mhInst;
}

const HWND& Framework::GetHWND()
{
    return mhWnd;
}

const RESOLUTION& Framework::GetResolution()
{
    return mResolution;
}

void Framework::Launch(ThreadName thname)
{
    switch (thname)
    {
    case ThreadName::main:
        Launch_main();
        break;
    case ThreadName::network:
        Launch_Network();
        break;
    }
}

int Framework::Launch_main()
{
    static HACCEL hAccelTable = LoadAccelerators(mhInst, MAKEINTRESOURCE(IDC_CHESS));
    static MSG msg{};

    while (mIsRun)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                continue;

            }
        }
        else
        {
            Logic();
        }
    }

    ::DestroyWindow(mhWnd);
    ::UnregisterClass(L"XMachina Server Stress Test", mhInst);

    return (int)msg.wParam;
}

void Framework::Launch_Network()
{
    THREAD_MGR->Launch([&]() {
        mNetService->Launch();
        });
}



