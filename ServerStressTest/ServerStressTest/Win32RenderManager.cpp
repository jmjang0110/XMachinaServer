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
	FillRect(mBackHDC, &size, (HBRUSH)(COLOR_WINDOW + 1));	// ����� ������� ĥ��
}


void Win32RenderMgr::Render_Present()
{
	// Back DC (BackBuffer RTV) �� �ִ� ȭ���� main HDC �� ����մϴ�... < ���� ��� >
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
		, static_cast<int>(lt.x), static_cast<int>(lt.y)							// ������ â ȭ�鿡���� ��ġ
		, static_cast<int>(size.x), static_cast<int>(size.y)						// ������ â ȭ�鿡���� ������
		, static_cast<int>(drawImage.LT.x), static_cast<int>(drawImage.LT.y)		// �̹��������� ��ġ ( �ȼ� )
		, static_cast<int>(drawImage.Size.x), static_cast<int>(drawImage.Size.y));	// �̹��������� ������ ( �ȼ� )

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


