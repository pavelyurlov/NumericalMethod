#pragma once

#include "Distribution1D.h"

class Fourier
{
public:
	static Distribution1D Convolution(Distribution1D, Distribution1D);
};