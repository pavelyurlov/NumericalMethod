#pragma once

#include "minitypes.h"
#include "Partition.h"
#include "Function.h"

class Distribution1D
{
public:
	Distribution1D(Partition&, num);
	Distribution1D(Partition&, Function);

	num CountIntegral();

	Distribution1D operator+ (Distribution1D&);
	Distribution1D operator- (Distribution1D&);
	Distribution1D operator* (Distribution1D&);
	Distribution1D operator/ (Distribution1D&);
	Distribution1D operator+ (num);
	Distribution1D operator* (num);
	friend Distribution1D operator+(num n, Distribution1D mv);
	friend Distribution1D operator*(num n, Distribution1D mv);
};
