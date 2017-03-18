#pragma once
#include "Fourier.h"

class FourierFFTW
{
public:
	static Distribution1D Convolution(Distribution1D, Distribution1D);
};

