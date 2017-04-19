// Win32Project1.cpp : Defines the entry point for the application.
//

#pragma comment (lib,"Gdiplus.lib")

#include "stdafx.h"
#include <Windows.h>
#include "atlimage.h"
#include <vector>
using namespace Gdiplus;
HWND                hWnd = NULL;
HWND                hWnd2 = NULL;
std::vector<HWND> mouses;

VOID OnPaint(HDC hdc)
{
	POINT p;
	GetCursorPos(&p);
	Graphics    graphics(hdc);
	Image image(L"cursor.ico");
	graphics.DrawImage(&image, p.x, p.y, image.GetWidth(), image.GetHeight());
}
VOID Update() {
	// Load our PNG image
	if (hWnd == NULL)
	{
		MessageBeep(ERROR);

	}
	CImage img;
	img.Load(L"cursor3.png");
	// Get dimensions
	int iWidth = img.GetWidth();
	int iHeight = img.GetHeight();
	// Make mem DC + mem  bitmap
	HDC hdcScreen = GetDC(0);
	HDC hDC = CreateCompatibleDC(hdcScreen);
	HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
	HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
	// Draw image to memory DC
	img.Draw(hDC, 0, 0, iWidth, iHeight, 0, 0, iWidth, iHeight);

	// Call UpdateLayeredWindow
	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;
	POINT ptPos = { 20, 20 };
	SIZE sizeWnd = { iWidth, iHeight };
	POINT ptSrc = { 0, 0 };
	UpdateLayeredWindow(hWnd, hdcScreen, &ptPos, &sizeWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);

	SelectObject(hDC, hBmpOld);
	DeleteObject(hBmp);
	DeleteDC(hDC);
	ReleaseDC(0, hdcScreen);


}

VOID OnPaint2(HDC hdc)
{
	Graphics    graphics(hdc);
	Image image(L"cursor.png");
	graphics.DrawImage(&image, 60, 10);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int iWidth = 0;
int iHeight = 0;

VOID op(HWND hWnd, int x, int y) {
	SetWindowPos(hWnd, 0, x, y, iWidth, iHeight, 0);
}

HWND setCursor(HINSTANCE hInstance, HWND hWnd, LPCWSTR className, LPCWSTR winName)
{
	WNDCLASS            wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className;

	RegisterClass(&wndClass);

	hWnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
		className,   // window class name
		TEXT("Getting Started"),  // window caption
		WS_POPUP | WS_VISIBLE | WS_SYSMENU,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		80,            // initial x size
		80,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);

	SetWindowLong(hWnd,
		GWL_EXSTYLE,
		GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	LONG lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle);

	LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
	lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
	SetWindowLong(hWnd, GWL_STYLE, lStyle);

	CImage img;
	img.Load(L"cursor3.png");

	// Get dimensions of Image
	iWidth = img.GetWidth();
	iHeight = img.GetHeight();

	// Make a memory DC + memory bitmap
	HDC hdcScreen = GetDC(NULL);
	HDC hDC = CreateCompatibleDC(hdcScreen);
	HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
	HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);

	// Draw image to memory DC
	img.Draw(hDC, 0, 0, iWidth, iHeight, 0, 0, iWidth, iHeight);

	// Call UpdateLayeredWindow
	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	POINT ptPos = { 0, 0 };
	SIZE sizeWnd = { iWidth, iHeight };
	POINT ptSrc = { 0, 0 };

	UpdateLayeredWindow(hWnd, hdcScreen, &ptPos, &sizeWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);

	SelectObject(hDC, hBmpOld);
	DeleteObject(hBmp);
	DeleteDC(hDC);
	ReleaseDC(NULL, hdcScreen);

	return hWnd;
}

VOID AddMouse(HWND hWnd) {
	mouses.push_back(hWnd);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	WNDCLASS            sndClass;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	hWnd = setCursor(hInstance, hWnd, L"yee", L"yee");

	AddMouse(hWnd);

	hWnd2 = setCursor(hInstance, hWnd2, L"yee2", L"yee2");

	AddMouse(hWnd2);

	SetWindowPos(hWnd, 0, 200, 200, iWidth, iHeight, 0);

	SetWindowPos(hWnd2, 0, 200, 200, iWidth, iHeight, 0);
	
	//ShowWindow(hWnd, iCmdShow);

	
	

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_MOUSEMOVE)
		{
			//UpdateWindow(hWnd);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}  // WinMain



MSLLHOOKSTRUCT * pMouseStruct;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;
	bool finished = true;

	switch (message)
	{
		
	case WM_PAINT:
		
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:

		return 0;
	case WM_MOUSEMOVE:

		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} 

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		MSLLHOOKSTRUCT * pMouseStruct = (MSLLHOOKSTRUCT *)lParam;
		;
		//UpdateLayeredWindow(hWnd, NULL, &ptPos, NULL, NULL, &ptPos, 0, NULL, ULW_ALPHA);
		//
		for (size_t i = 0; i < mouses.size(); i++) {		
			if (i == 1) {
				op(mouses.at(i), pMouseStruct->pt.x + 100, pMouseStruct->pt.y + 100);
			}
			else {
				op(mouses.at(i), pMouseStruct->pt.x + 100, pMouseStruct->pt.y - 100);
			}
		}
		
		//op(hWnd2, pMouseStruct->pt.x + 200, pMouseStruct->pt.y + 200);
	}
	
	return CallNextHookEx(0, nCode, wParam, lParam);
}

HHOOK mousehook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);

