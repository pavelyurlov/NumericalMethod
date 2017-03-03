#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Numerical_Lib\InputParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Num_UnitTest
{		
	TEST_CLASS(InputUnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::vector<InputSet> res = InputParser::Parse(".\TestFiles\1.json");
			Assert::AreEqual((float)res[0].b1, (float)4, L"Обычное чтение из файла", LINE_INFO());
		}

	};
}