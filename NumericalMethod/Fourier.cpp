#define _USE_MATH_DEFINES
#include <math.h>
#include "MatlabVector.h"
#include <complex>

#include <mkl_types.h>
#include <mkl.h>
#include <mkl_dfti.h>

#include "Preferences.h"
#include <specialfunctions.h>
#include <gsl\gsl_dht.h>

// быстрое одномерное преобразование Фурье векторов a и b ака численно заданных функций
// вектор a и вектор b должны быть одинакового размера
MatlabVector conv_fourier_lib(MatlabVector a, MatlabVector b)
{
	static uint size_a = 0, size_b = 0;
	static MKL_Complex16 *nA, *nB, *fA, *fB;
	static DFTI_DESCRIPTOR_HANDLE fpa, fpb, bpa;

	if (size_a != a.size() || size_b != b.size())
	{
		size_a = a.size(); size_b = b.size();
		DftiFreeDescriptor(&fpa); DftiFreeDescriptor(&bpa); DftiFreeDescriptor(&fpb);
		if (nA) mkl_free(nA);
		if (nB) mkl_free(nB);
		if (fA) mkl_free(fA);
		if (fB) mkl_free(fB);

		nA = (MKL_Complex16*)mkl_malloc(sizeof(MKL_Complex16) * size_a, 0);
		nB = (MKL_Complex16*)mkl_malloc(sizeof(MKL_Complex16) * size_b, 0);
		fA = (MKL_Complex16*)mkl_malloc(sizeof(MKL_Complex16) * size_a, 0);
		fB = (MKL_Complex16*)mkl_malloc(sizeof(MKL_Complex16) * size_b, 0);

		DftiCreateDescriptor(&fpa, DFTI_DOUBLE, DFTI_COMPLEX, 1, size_a);
		DftiCreateDescriptor(&fpb, DFTI_DOUBLE, DFTI_COMPLEX, 1, size_b);
		DftiCreateDescriptor(&bpa, DFTI_DOUBLE, DFTI_COMPLEX, 1, size_a);
		DftiCommitDescriptor(fpa);
		DftiCommitDescriptor(fpb);
		DftiCommitDescriptor(bpa);
	}

	for (uint i = 0; i < a.size(); i++)
	{
		nA[i].real = a[i];
		nA[i].imag = 0;
	}
	DftiComputeForward(fpa, nA, fA);

	for (uint i = 0; i < b.size(); i++)
	{
		nB[i].real = b[i];
		nB[i].imag = 0;
	}
	DftiComputeForward(fpb, nB, fB);

	if (a.size() != b.size()) throw new char*("a.size != b.size");
	for (uint i = 0; i < a.size(); i++) // выход за пределы, если массивы разного размера. Но этого не должно случиться.
	{
		auto tmp_compl = std::complex<num>(fA[i].real, fA[i].imag) * std::complex<num>(fB[i].real, fB[i].imag);
		fA[i].real = tmp_compl.real();
		fA[i].imag = tmp_compl.imag();
	}

	DftiComputeBackward(bpa, fA, nA);

	MatlabVector res = MatlabVector(a.size());
	for (uint i = 0; i < a.size(); i++)
	{
		a[i] = nA[i].real;
	}

	return res;
}

// прямое преобразование Ханкеля
MatlabVector hankel_2D_direct(MatlabVector f)
{
	MatlabVector res(f.size());
	for (uint i = 0; i < res.size(); i++)
	{
		res[i] = 0;
		MatlabVector tmp = g_rh * f;
		for (uint j = 0; j < tmp.size(); j++)
		{
			res[i] += tmp[j] * alglib::besselj0((g_rh)[j] * (g_rh)[i]);
		}
	}
	return res;
}

MatlabVector hankel_2D_gsl(MatlabVector f)
{
	static uint the_size = 0; // /2
	static gsl_dht *the_dht;
	static double *the_in;
	static double *the_out;

	MatlabVector res = MatlabVector(f.size());

	if (the_size != 0 && the_size != f.size())
	{
		gsl_dht_free(the_dht);
		the_size = 0;
		delete the_in;
		delete the_out;
	}

	if (the_size == 0)
	{
		the_size = f.size();
		the_dht = gsl_dht_new(the_size / 2 + the_size % 2, 0, g_A);
		the_in = new double[the_size / 2 + the_size % 2];
		the_out = new double[the_size / 2 + the_size % 2];
	}

	for (int i = 0; i < the_size / 2 + the_size % 2; i++)
	{
		the_in[the_size / 2 + the_size % 2 - i - 1] = f[i];
	}

	gsl_dht_apply(the_dht, the_in, the_out);

	for (int i = 0; i < the_size / 2 + the_size % 2; i++)
	{
		res[the_size / 2 + the_size % 2 - i - 1] = res[the_size / 2 + i] = the_out[i];
	}
	return res;
}

// двумерное преобразование Фурье радиально-симметричных функций
// середина входного вектора считается точкой ноль
MatlabVector conv_radial_2D(MatlabVector a, MatlabVector b)
{
	MatlabVector res;
	// res = \frac{ 1 }{2\pi} H[(2\pi) ^ 2 H[f] \cdot H[g]]
	// so, we need H
	res = hankel_2D_gsl(hankel_2D_gsl(a) * hankel_2D_gsl(b) * 4 * M_PI * M_PI) * (1 / (2 * M_PI));
	return res;
}

MatlabVector conv_1d_mkl(MatlabVector a, MatlabVector b)
{
	static uint size_a = 0, size_b = 0;
	static VSLConvTaskPtr convolution_ptr = NULL;
	if (size_a != a.size() || size_b != b.size())
	{
		size_a = a.size(); size_b = b.size();
		vslConvDeleteTask(&convolution_ptr); // утечки?
		vsldConvNewTask1D(&convolution_ptr, VSL_CONV_MODE_AUTO, size_a, size_b, size_a);
	}
	MatlabVector res = a;
	vsldConvExec1D(convolution_ptr, a.data(), 1, b.data(), 1, res.data(), 1);
	return res;
}

MatlabVector conv_2d_mkl(MatlabVector a, MatlabVector b)
{
	static uint size_a = 0, size_b = 0;
	static VSLConvTaskPtr convolution_ptr = NULL;
	int shape[2] = { g_N , 1 };
	if (size_a != a.size() || size_b != b.size())
	{
		size_a = a.size(); size_b = b.size();
		vslConvDeleteTask(&convolution_ptr); // утечки?
		vsldConvNewTask(&convolution_ptr, VSL_CONV_MODE_AUTO, 2, shape, shape, shape);
	}
	MatlabVector res = a;
	vsldConvExec(convolution_ptr, a.data(), shape, b.data(), shape, res.data(), shape);
	return res;
}

MatlabVector conv_3d(MatlabVector a, MatlabVector b)
{
	return 4 * M_PI * conv_1d_mkl(g_rh.getModule() * a, b); // [f *** g] = 4pi [(rf) * g]
}

// внешний "интерфейс"
// основная программа вызывает эту функцию
MatlabVector conv(MatlabVector a, MatlabVector b, int) // int в конце несущественнен
{
	switch (preferences.dimentions)
	{
	case 1:
		return conv_1d_mkl(a, b);
	case 2:
		return conv_2d_mkl(a, b);
	case 3:
		return conv_3d(a, b);
	default:
		throw 123;
	}
}