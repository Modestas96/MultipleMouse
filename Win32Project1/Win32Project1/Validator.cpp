#include <iostream>
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
struct Validator {
	Validator() {

	}

	bool ok(int x, int y) {
		RECT display;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &display, 0);

		if (x <= display.right && x >= display.left && y <= display.bottom && y >= display.top)
		{
			return true;
		}
		return false;

	}
};