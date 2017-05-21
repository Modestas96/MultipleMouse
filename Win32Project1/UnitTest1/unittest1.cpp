#include "stdafx.h"
#include "CppUnitTest.h"
#include "Resource.h"
#include "Win32Project1.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(sum)
		{
			// TODO: Your test code here
			Assert::AreEqual(1, 1);
		}

	};
}