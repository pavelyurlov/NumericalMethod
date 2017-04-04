#pragma once

#include "IOSets.h"

/*!
@brief Вычисление одного набора входных данных
@details Класс, получающий один набор входных данных, вычисляющий его и отдающий результат.
*/
class NumericalMethod
{
public:
	NumericalMethod(InputSet input);
	void Count();
	OutputSet GetResult();
private:
	const InputSet c_input;
	OutputSet m_result;
	Partition & m_part;
};