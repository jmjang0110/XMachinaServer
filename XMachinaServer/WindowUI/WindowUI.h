#pragma once

/// +-------------------------------
///			   Window UI
/// ________________________________
///			윈도우 창 관리 
/// -------------------------------+

/* 화면 해상도 */
typedef struct WindowResolution
{
	INT32 Width  = {};
	INT32 Height = {};

}WResolution, *PWResolution;

/* Window UI */
#define WINDOW_UI WindowUI::GetInst()
class WindowUI
{
	DECLARE_SINGLETON(WindowUI);

private:
	static HINSTANCE	mhInst;
	static HWND			mHwnd;
	static HACCEL		mAccelTable;
	static MSG			mMsg;

private:
	WResolution			mRes{};
	BYTE				mIsRun = 0x01; // true 


public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	WindowUI();
	~WindowUI();

public:
	static constexpr HWND& GetHwnd() { return mHwnd; }

public:
	void Launch();

	bool Init(HINSTANCE& hInstance, WindowResolution Res);
	bool CreateWindowUI();
	int  Loop();	 // window while loop 
	void Logic();	 // main logic



};

