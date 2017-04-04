#pragma once
#include "Fourier.h"

/*!
@brief Свёртка с помощью библиотеки MKL
@details Статический класс, реализующий свёртку с помощью библиотеки MKL
@todo Сделать динамическим и сделать единый интерфейс свёрток
*/
class FourierMKL
{
public:
	/*!
	@brief Свёртка
	@param[in] a,b Два распределения с одинаковыми разбиениями, которые будут свёрнуты.
	@result Распределение, содержащее результат свёртки.
	*/
	static Distribution1D Convolution(Distribution1D a, Distribution1D b);
};

