// Win32Project1.cpp : Defines the entry point for the application.
//

#pragma comment (lib,"Gdiplus.lib")


#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "atlimage.h"
#include <vector>
#include "mouseDevice.h"
#include <iostream>
#include <Winuser.h>

#include "interception.h"
#include "utils.h"

using namespace Gdiplus;

#define STATE_LEFT true;
#define STATE_RIGHT false;


HWND                hWnd = NULL;
HWND                hWnd2 = NULL;
std::vector<mouseDevice> mouseDevices;
int MouseCount = 1;


INT WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow);
void handleDevices(HINSTANCE);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void handleMouseEvent(int, int, short, short);
bool checkIfInDisplayY(HWND hwnd, int x, int y);
bool checkIfInDisplayX(HWND hwnd, int x, int y);
int sum(int, int);

int iWidth = 0;
int iHeight = 0;
int mouseCount = 0;

struct ts {
	int a, b;
};

int sum(ts s) {
	return s.a + s.b;
}
int sum(int a) {
	return a;
}
VOID updateCurPosition(mouseDevice device) {
	//printf("%s", device.name);
	SetWindowPos(device.hWnd, HWND_TOPMOST, device.x, device.y, iWidth, iHeight, 0);
}

HWND setCursor(HINSTANCE hInstance, HWND hWnd, LPCWSTR className, LPCWSTR winName, int id)
{
	WNDCLASS            wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className;
	
	RegisterClass(&wndClass);
	CImage img;
	if (id == 1 || id == 0)
	{
		img.Load(L"Cursors/Purple.png");
	}
	else if (id == 2)
	{
		img.Load(L"Cursors/Red.png");
	}
	else if (id == 3)
	{
		img.Load(L"Cursors/Orange.png");
	}
	else if (id == 4)
	{
		img.Load(L"Cursors/Orange.png");
	}
	else if (id == 5)
	{
		img.Load(L"Cursors/Blue.png");
	}
	
	
	iWidth = img.GetWidth();
	iHeight = img.GetHeight();

	hWnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
		className,   // window class name
		winName,  // window caption
		WS_POPUP | WS_VISIBLE | WS_SYSMENU,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		iWidth,            // initial x size
		iHeight,            // initial y size
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

HWND AddMouse(HINSTANCE hInstance, InterceptionDevice device, int x, int y, int id) {
	HWND hWnd = NULL;
	if(mouseDevices.size() != 0)
		hWnd = setCursor(hInstance, hWnd, TEXT("Mouse"), TEXT("Mouse"), id);
	mouseDevices.push_back(mouseDevice(hWnd, device, x, y));
	MouseCount++;
	return hWnd;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	// CIa kad console issoktu lol1!!
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf("debug mode XD \n");
	///////
	MSG                 msg;
	
	handleDevices(hInstance);
	

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

bool isLeagalX(int x, int y, HWND hwnd) {
	RECT display;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &display, 0);
	
	if (x <= display.right && x >= display.left)
	{
		return true;
	}
	return checkIfInDisplayX(hwnd, x, y);
}

bool isLeagalY(int x, int y, HWND hwnd) {
	RECT display;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &display, 0);

	if (y <= display.bottom + iHeight + 10 && y >= display.top)
	{
		return true;
	}

	return checkIfInDisplayY(hwnd, x, y);
}

bool checkIfInDisplayX(HWND hwnd, int x, int y) {
	HWND op = hWnd;
	SetWindowPos(hwnd, HWND_TOPMOST, x, y, iWidth, iHeight, 0);
	HMONITOR otherMonitors;
	MONITORINFO mi;
	RECT prc;
	RECT rc;
	GetWindowRect(hwnd, &prc);
	otherMonitors = MonitorFromRect(&prc, MONITOR_DEFAULTTONEAREST);
	mi.cbSize = sizeof(mi);

	GetMonitorInfo(otherMonitors, &mi);

	rc = mi.rcWork;
	
	if (x <= rc.right && x >= rc.left)
	{
		return true;
	}

	return false;
}

bool checkIfInDisplayY(HWND hwnd, int x, int y) {
	HWND op = hWnd;
	SetWindowPos(hwnd, HWND_TOPMOST, x, y, iWidth, iHeight, 0);
	HMONITOR otherMonitors;
	MONITORINFO mi;
	RECT prc;
	RECT rc;
	GetWindowRect(hwnd, &prc);
	otherMonitors = MonitorFromRect(&prc, MONITOR_DEFAULTTONEAREST);
	mi.cbSize = sizeof(mi);

	GetMonitorInfo(otherMonitors, &mi);

	rc = mi.rcWork;
	if (y <= rc.bottom + iHeight + 10 && y >= rc.top)
	{
		return true;
	}

	return false;
}



void handleDevices(HINSTANCE hInstance) {
	InterceptionContext context;
	InterceptionDevice device;
	InterceptionStroke stroke;

	//raise_process_priority();

	context = interception_create_context();

	//interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
	interception_set_filter(context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_MOVE | INTERCEPTION_FILTER_MOUSE_ALL);
	bool op = true;

	while (interception_receive(context, device = interception_wait(context), &stroke, 1) > 0)
	{

		bool fnd = false;
		if (interception_is_mouse(device))
		{
			InterceptionMouseStroke &mousestroke = *(InterceptionMouseStroke *)&stroke;


			mouseDevice now = mouseDevice();

			int index = 0;
			for (int i = 0; i < mouseDevices.size(); i++) {
				
				if (mouseDevices[i].name == device) {
					
					now = mouseDevices[i];
					index = i;
					fnd = true;

					if (i == 0) continue;
					

					if (isLeagalX(mouseDevices[i].x + mousestroke.x, mouseDevices[i].y + mousestroke.y, mouseDevices[i].hWnd)) {
						mouseDevices[i].x += mousestroke.x;
					}
					if (isLeagalY(mouseDevices[i].x + mousestroke.x, mouseDevices[i].y + mousestroke.y, mouseDevices[i].hWnd)) {
						mouseDevices[i].y += mousestroke.y;
					}

					printf("uid: %d, x: %d, y : %d \n", i + 1, mouseDevices[i].x, mouseDevices[i].y);
					printf("%d", mousestroke.rolling);

					if (mousestroke.state == 2) {
						handleMouseEvent(mouseDevices[i].x, mouseDevices[i].y, mousestroke.state, 0);
					}
					if (mousestroke.state == 8) {
						handleMouseEvent(mouseDevices[i].x, mouseDevices[i].y, mousestroke.state, 0);
					}
					if (mousestroke.state == 32) {
						handleMouseEvent(mouseDevices[i].x, mouseDevices[i].y, mousestroke.state, 0);
					}
					if (mousestroke.state == 1024) {
						handleMouseEvent(mouseDevices[i].x, mouseDevices[i].y, mousestroke.state, mousestroke.rolling);
					}
				}
			}
			if (!fnd) {
				
				fnd = false;
				AddMouse(hInstance, device, mousestroke.x, mousestroke.y, mouseCount);
				mouseCount++;
			}
			if (device == mouseDevices[0].name) {
				interception_send(context, device, &stroke, 2);
			}
			else {
				updateCurPosition(now);
				//updateCursorPosition(now, mousestroke.x, mousestroke.y, index);
			}
		}

	}


	interception_destroy_context(context);

}

void handleMouseEvent(int x, int y, short state, short rolling) {

	const double XSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
	const double YSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);


	//printf("metrics %d", GetSystemMetrics(SM_CXSCREEN) - 1);
	printf("HEYYY %d %d", x, y);
	printf("state %d \n", state);
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dx = x * XSCALEFACTOR;
	Input.mi.dy = y * YSCALEFACTOR;


	if (state == 2)
	{
		Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	}
	if (state == 8)
	{
		Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
	}
	if (state == 32)
	{
		Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP;
	}
	if (state == 1024)
	{
		Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_WHEEL;
		Input.mi.mouseData = WHEEL_PAGESCROLL * rolling * -1;
	}
	

	

	POINT p;
	GetCursorPos(&p);


	SendInput(1, &Input, sizeof(INPUT));

	SetCursorPos(p.x, p.y);

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
