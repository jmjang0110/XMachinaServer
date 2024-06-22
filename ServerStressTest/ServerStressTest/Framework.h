#pragma once
#include "NetworkService.h"

#define FRAMEWORK Framework::GetInst()

class Object;

enum class ThreadName
{
	main,
	network,
};

class Framework
{
	DECLARE_SINGLETON(Framework);

private:
	static BYTE	mIsRun;

private:
	RESOLUTION	mResolution;				// 해상도	

	HINSTANCE	mhInst;
	HWND		mhWnd;

	class Scene* mGameScene{};

	/* Network Service */
	NetworkService* mNetService;

	std::mutex  mNewObjEventLock;
	std::queue<Object*> mNewObjectEvents;

public:
	Framework();
	~Framework();

public:
	void ProcessEvent();
	void PushNewObjEvent(Object* obj);


	bool	Init(HINSTANCE& hInstance, RESOLUTION Res);
	bool	Create();

	int		Loop();
	void	Logic();

	const HINSTANCE& GetHINSTANCE();
	const HWND& GetHWND();
	const RESOLUTION& GetResolution();
	Scene* GetGameScene() { return mGameScene; }
	NetworkService* GetNerworkService() { return mNetService; }

public:
	void Launch(ThreadName thname);
	int  Launch_main();
	void Launch_Network();

	void SendServerPacket();


public:
	void ChangeWindowSize(HWND _hwnd, const RESOLUTION& res);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);		// 정보 대화 상자의 메시지 처리기입니다.
};

