#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Numerical_Lib\IOSets.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Num_UnitTest
{
	TEST_CLASS(IOSetUnitTest)
	{
	public:
		TEST_METHOD(IOSet__InsertN)
		{
			uint correct = 5;
			InputSet is;
			is.insert("N", 5);
			Assert::AreEqual(correct, is.N, L"Вставка в поле N");
		}
	};
}