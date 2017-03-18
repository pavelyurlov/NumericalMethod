#include "FourierMKL.h"

#include <mkl_types.h>
#include <mkl.h>
#include <mkl_dfti.h>


Distribution1D FourierMKL::Convolution(Distribution1D a, Distribution1D b)
{
	static uint size_a = 0, size_b = 0;
	static VSLConvTaskPtr convolution_ptr = NULL;
	if (size_a != a.GetSize() || size_b != b.GetSize())
	{
		size_a = a.GetSize(); size_b = b.GetSize();
		vslConvDeleteTask(&convolution_ptr); // утечки?
		vsldConvNewTask1D(&convolution_ptr, VSL_CONV_MODE_AUTO, size_a, size_b, size_a);
	}
	Distribution1D res = a; // TODO: проверить копирование. Лучше сделать через прямое создание
	vsldConvExec1D(convolution_ptr, a.GetData(), 1, b.GetData(), 1, res.GetData(), 1);
	return res;
}