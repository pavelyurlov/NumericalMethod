#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "minitypes.h"

/*!
@brief Старая функция для вычисления нормального распределения
@details Ныне заменена на NormalFunction1D. Причина замены: не была унифицирована, что затрудняло подстановку другой функции на её место.
*/
num normpdf_checked(num r, num sigma, uint dim)
{
	return exp(-pow(r, 2) / (pow(2 * sigma, 2))) / (pow(pow(2 * M_PI, 0.5)*sigma, dim));
}