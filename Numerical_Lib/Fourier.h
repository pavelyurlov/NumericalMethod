#pragma once

#include "Distribution1D.h"

// TODO: в файлах Fourier.h и Fourier.cpp остался только архивный мусор
class Fourier
{
public:
	virtual Distribution1D Convolution(Distribution1D, Distribution1D) = 0;
};