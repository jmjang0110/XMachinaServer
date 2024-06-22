#include "pch.h"
#include "Win32RenderManager.h"
#include "Framework.h"

DEFINE_SINGLETON(Win32RenderMgr);

Win32RenderMgr::Win32RenderMgr()
{

}

Win32RenderMgr::~Win32RenderMgr()
{
	SelectObject(mFrontHDC, mBackHDC);
	ReleaseDC(mhWnd, mFrontHDC);
}

bool Win32RenderMgr::Init(RESOLUTION& Res, HWND& hwnd)
{
	mResolution = Res;
	mhWnd = hwnd;

	// Double buffering 
	mFrontHDC = GetDC(mhWnd);
	mBackHDC = CreateCompatibleDC(mFrontHDC);

	mBackBuffer = CreateCompatibleBitmap(mFrontHDC, mResolution.Width, mResolution.Height);
	mPrevBackBuffer = static_cast<HBITMAP>(SelectObject(mBackHDC, mBackBuffer));

	return true;
}



void Win32RenderMgr::Render_Prepare()
{
	static RECT size = RECT{ 0,0, (long)mResolution.Width, (long)mResolution.Height };
	FillRect(mBackHDC, &size, (HBRUSH)(COLOR_WINDOW + 1));	// 배경을 흰색으로 칠함
}


void Win32RenderMgr::Render_Present()
{
	// Back DC (BackBuffer RTV) 에 있는 화면을 main HDC 에 출력합니다... < 최종 출력 >
	BitBlt(mFrontHDC, 0, 0, mResolution.Width, mResolution.Height, mBackHDC, 0, 0, SRCCOPY);
}

constexpr HDC& Win32RenderMgr::GetRenderHDC()
{
	return mBackHDC;
}

void Win32RenderMgr::DrawLine(const POINT& start, const POINT& end)
{
	::MoveToEx(mBackHDC, start.x, start.y, NULL);
	::LineTo(mBackHDC, end.x, end.y);
}

void Win32RenderMgr::DrawPolygon(const POINT* apt, int cpt)
{
	::Polygon(mBackHDC, apt, cpt);
}

void Win32RenderMgr::DrawImage(Image& drawImage, VECTOR2 lt, VECTOR2 size)
{
	drawImage.image.Draw(mBackHDC
		, static_cast<int>(lt.x), static_cast<int>(lt.y)							// 윈도우 창 화면에서의 위치
		, static_cast<int>(size.x), static_cast<int>(size.y)						// 윈도우 창 화면에서의 사이즈
		, static_cast<int>(drawImage.LT.x), static_cast<int>(drawImage.LT.y)		// 이미지에서의 위치 ( 픽셀 )
		, static_cast<int>(drawImage.Size.x), static_cast<int>(drawImage.Size.y));	// 이미지에서의 사이즈 ( 픽셀 )

}

void Win32RenderMgr::SetPen(int style, int width, COLORREF color)
{
	mPrevPen = ::CreatePen(style, width, color);
	mPen = ::SelectObject(mBackHDC, mPrevPen);

}

void Win32RenderMgr::SetOriginPen()
{
	if (mPrevPen) {
		::DeleteObject(mPrevPen);
	}
	::SelectObject(mBackHDC, mPen);
}


