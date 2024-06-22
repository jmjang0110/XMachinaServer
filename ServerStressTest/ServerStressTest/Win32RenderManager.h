#pragma once

#include "Object.h"
#define RENDER_MGR Win32RenderMgr::GetInst()


class Win32RenderMgr
{
	DECLARE_SINGLETON(Win32RenderMgr);

private:
	RESOLUTION	mResolution{};					// 해상도	
	HWND		mhWnd{};

	HDC			mFrontHDC{};					// 최종 출력 HDC 
	HDC			mBackHDC{};						// 백버퍼 HDC 

	HBITMAP		mBackBuffer{};
	HBITMAP		mPrevBackBuffer{};

	HGDIOBJ		mPen{};

	HPEN		mPrevPen{};

public:
	Win32RenderMgr();
	~Win32RenderMgr();

	bool Init(RESOLUTION& Res, HWND& hwnd);
	void Render_Prepare();
	void Render_Present();

	constexpr HDC& GetRenderHDC();

public:
	void DrawLine(const POINT& start, const POINT& end);
	void DrawPolygon(const POINT* apt, int cpt);
	void DrawImage(Image& image, VECTOR2 lt, VECTOR2 size);


public:
	void SetPen(int style, int width, COLORREF color);
	void SetOriginPen();


};

