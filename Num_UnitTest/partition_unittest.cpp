#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Numerical_Lib\Partition.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Num_UnitTest
{
	TEST_CLASS(PartitionUnitTest)
	{
	public:
		TEST_METHOD(GetOne)
		{
			Partition::GetPartition(10, 20);
		}

		TEST_METHOD(GetDifferent)
		{
			Partition::GetPartition(10, 20);
			Partition::GetPartition(5, 10);
		}

		TEST_METHOD(GetSame)
		{
			Partition::GetPartition(10, 20);
			Partition::GetPartition(10, 20);
		}
	};
}