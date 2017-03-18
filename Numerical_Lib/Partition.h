#pragma once

#include "minitypes.h"
#include <vector>

// Равномерное разбиение
class Partition
{
public:
	static Partition& GetPartition(num area_size, uint num_of_points);	// середина отрезка - в нуле
	num GetStep();
private: // funcs
	Partition(uint id, num area_size, uint num_of_points);
	bool IsAlike(num area_size, uint num_of_points);
private: // vars
	const uint c_id;
	const num c_a; // area_size
	const num c_n; // num_of_points
private: // static
	static std::vector<Partition> s_all;
};

