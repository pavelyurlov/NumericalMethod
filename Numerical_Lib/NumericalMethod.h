#pragma once

#include "IOSets.h"

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