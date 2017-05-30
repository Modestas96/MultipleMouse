
#include "stdafx.h"
#include "C:\Users\ka\MultipleMouse\Win32Project1\Win32Project1\Validator.cpp"

#include "C:\Users\ka\MultipleMouse\Win32Project1\Win32Project1\ScreenStruct.cpp"
#include <Winuser.h>
#include <Windows.h> 
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{
	TEST_CLASS(UnitTest2)
	{
	public:

		Validator v;
		ScreenStruct s;
		int sW = s.getScreenW();
		int sH = s.getScreenH();
		TEST_METHOD(IN_SCREEN)
		{
			Assert::AreEqual(v.ok(sW - 10, sH - 20), true);
			Assert::AreEqual(v.ok(sW / 2, sH / 2), true);
			Assert::AreEqual(v.ok(sW / 2, sH / 2), true);
			Assert::AreEqual(v.ok(sW - 366, sH - 123), true);
			Assert::AreEqual(v.ok(sW / 7, sH / 2), true);
			Assert::AreEqual(v.ok(sW / 2, sH / 7), true);
		}
		TEST_METHOD(OUT_OF_SCREEN)
		{
			Assert::AreEqual(v.ok(sW + 10, sH + 5), false);
			Assert::AreEqual(v.ok(sW * 2, sH * 2), false);
			Assert::AreEqual(v.ok(sW * 2, sH * 2), false);
			Assert::AreEqual(v.ok(sW + 366, sH + 123), false);
			Assert::AreEqual(v.ok(sW * 7, sH * 2), false);
			Assert::AreEqual(v.ok(sW * 2, sH * 7), false);
		}
		TEST_METHOD(ON_BOUNDARY)
		{
			Assert::AreEqual(v.ok(0, 0), true);
			Assert::AreEqual(v.ok(1, 1), true);
			Assert::AreEqual(v.ok(sW, sH), false);
			Assert::AreEqual(v.ok(sW-1, sH-1), true);
		}

	};
}