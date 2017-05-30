#include <iostream>

#pragma comment (lib,"Gdiplus.lib")


#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "atlimage.h"
#include <vector>
#include <iostream>
#include <Winuser.h>

#include "interception.h"
#include "utils.h"

using namespace Gdiplus;
struct ScreenStruct {
	Rect r;
	
	int getScreenW() {
		RECT display;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &display, 0);
		return display.right - display.left + 1;
	}
	int getScreenH() {
		RECT display;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &display, 0);
		return display.bottom - display.top + 1;
	}


};