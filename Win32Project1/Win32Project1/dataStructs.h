#include <string>
#include "interception.h"
using namespace std;

typedef struct mouseDevice {
	HWND hWnd;
	InterceptionDevice name;
	int x, y;
	mouseDevice() {}
	mouseDevice(HWND hWnd, InterceptionDevice name) {
		x = 0;
		y = 0;
		this->name = name;
		this->hWnd = hWnd;
	}

	mouseDevice(HWND hWnd,InterceptionDevice name, int x, int y) {
		this->name = name;
		this->x = x;
		this->y = y; 
		this->hWnd = hWnd;
	}
};