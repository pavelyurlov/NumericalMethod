#pragma once

#include "Distribution1D.h"

// TODO: в файлах Fourier.h и Fourier.cpp остался только архивный мусор

/*!
@brief Старый ненужный класс
@details Лежит, дожидаясь окончательного переноса кода из него в новые классы, после чего будет удалён
*/
class Fourier
{
public:
	virtual Distribution1D Convolution(Distribution1D, Distribution1D) = 0;
};