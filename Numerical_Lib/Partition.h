#pragma once

#include "minitypes.h"
#include <vector>

// Равномерное разбиение
/*!
@brief Равномерное разбиение
@details Класс, представляющий одномерное равномерное разбиение с центром в точке 0.
Используется для нахождения координат точек Distribution1D.
@todo Переименовать в Partition1D
*/
class Partition
{
public:
	_declspec(dllimport) static Partition& GetPartition(num area_size, uint num_of_points);	// середина отрезка - в нуле
	num GetStep();
	uint GetNum();
	num ItoX(uint); // get x corresponding to the index in the vector
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

