#include "stdafx.h"
#include "atlimage.h"
#include <vector>

using namespace Gdiplus;

HWND                hWnd = NULL;
HWND                hWnd2 = NULL;
std::vector<HWND> mouses;
int MouseCount = 0;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int iWidth = 0;
int iHeight = 0;

VOID updateCurPosition(HWND hWnd, int x, int y) {
	SetWindowPos(hWnd, HWND_TOPMOST, x, y, iWidth, iHeight, 0);
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
		winName,  // window caption
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

HWND AddMouse(HINSTANCE hInstance) {
	HWND hWnd = NULL;
	hWnd = setCursor(hInstance, hWnd, TEXT("Mouse" + MouseCount), TEXT("Mouse" + MouseCount)); // Cia tas +MouseCount neveikia reikia pataisyt lol
	mouses.push_back(hWnd);
	MouseCount++;
	return hWnd;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	MSG                 msg;

	AddMouse(hInstance);
	AddMouse(hInstance);


	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
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
		for (size_t i = 0; i < mouses.size(); i++) {
			if (i == 1) {
				updateCurPosition(mouses.at(i), pMouseStruct->pt.x + 100, pMouseStruct->pt.y + 100);
			}
			else {
				updateCurPosition(mouses.at(i), pMouseStruct->pt.x + 100, pMouseStruct->pt.y - 100);
			}
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

HHOOK mousehook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);

