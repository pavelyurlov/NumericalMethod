#include "Function.h"

#define _USE_MATH_DEFINES
#include <math.h>

NormalFunction1D::NormalFunction1D(num param, num sigma) :
	m_param(param), m_sigma(sigma) {}

num NormalFunction1D::At(num x) const
{
	return m_param * exp(-pow(x, 2) / (pow(2 * m_sigma, 2))) / (pow(2 * M_PI, 0.5)*m_sigma);
}
