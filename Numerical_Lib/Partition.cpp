#include "Partition.h"

std::vector<Partition> Partition::s_all = std::vector<Partition>();

Partition::Partition(uint id, num area_size, uint num_of_points):
	c_id(id), c_a(area_size), c_n(num_of_points) {}

Partition& Partition::GetPartition(num area_size, uint num_of_points)
{
	// найти похожее
	for (Partition & part : s_all)
	{
		if (part.IsAlike(area_size, num_of_points)) return part;
	}
	// если не нашли, делаем новое
	s_all.push_back(Partition(s_all.size(), area_size, num_of_points));
	return s_all.back();
}

bool Partition::IsAlike(num area_size, uint num_of_points)
{
	return c_a == area_size && c_n == num_of_points; // TODO: Test different and same, especially same
}

num Partition::GetStep()
{
	return c_a / (c_n - 1);
}

uint Partition::GetNum()
{
	return c_n;
}

num Partition::ItoX(uint i)
{
	return -(c_a / 2) + i * GetStep();
}