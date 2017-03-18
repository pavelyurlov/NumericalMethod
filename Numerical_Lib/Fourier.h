#pragma once

#include "Distribution1D.h"

class Fourier
{
public:
	static Distribution1D Convolution(Distribution1D, Distribution1D);
private:
	static Distribution1D Convolution1D_MKL(Distribution1D a, Distribution1D b);
	static Distribution1D Convolution1D_FFTW(Distribution1D a, Distribution1D b);
};