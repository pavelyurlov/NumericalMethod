#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Numerical_Lib\Distribution1D.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Num_UnitTest
{
	TEST_CLASS(Distribution1DUnitTest)
	{
	public:
		//******************COMPARE*********************//
		TEST_METHOD(Dist1D_CompareEqualConsts)
		{
			Partition p = Partition::GetPartition(1, 1000);

			Distribution1D a(p, 15);
			Distribution1D b(p, 15);
			Assert::AreEqual(true, a==b, L"Distribution 1D -- Compare op -- Equal Consts are not Equal!");
		}

		TEST_METHOD(Dist1D_CompDiffConsts)
		{
			Partition p = Partition::GetPartition(1, 1000);

			Distribution1D a(p, 15);
			Distribution1D b(p, 16);
			Assert::AreEqual(false, a == b, L"Distribution 1D -- Compare op -- Not Equal Consts are Equal!");
		}

		TEST_METHOD(Dist1D_CompSameFunc)
		{
			Partition p = Partition::GetPartition(1, 1000);

			Distribution1D a(p, NormalFunction1D(1, 1));
			Distribution1D b(p, NormalFunction1D(1, 1));
			Assert::AreEqual(true, a == b, L"Distribution 1D -- Compare op -- Equal Funcs are not Equal!");
		}

		TEST_METHOD(Dist1D_CompDiffFunc)
		{
			Partition p = Partition::GetPartition(1, 1000);

			Distribution1D a(p, NormalFunction1D(1, 1));
			Distribution1D b(p, NormalFunction1D(1, 0.5));
			Assert::AreEqual(false, a == b, L"Distribution 1D -- Compare op -- Not Equal Funcs are Equal!");
		}

		TEST_METHOD(Dist1D_CompSlightlyDiffFunc)
		{
			Partition p = Partition::GetPartition(1, 1000);

			Distribution1D a(p, NormalFunction1D(1, 1));
			Distribution1D b(p, NormalFunction1D(1.000000000001, 1.000000001));
			Assert::AreEqual(true, a == b, L"Distribution 1D -- Compare op -- Almost Same Funcs doesn't Pass!");
		}
	};
}