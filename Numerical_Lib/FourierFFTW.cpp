#ifdef FFTW
#include "FourierFFTW.h"

#include <fftw3.h>
#include <complex>


Distribution1D Fourier::Convolution(Distribution1D a, Distribution1D b)
{
	static uint size_a = 0, size_b = 0;
	static fftw_complex *nA, *nB, *fA, *fB; // n - normal, f - transformed
	static fftw_plan fpa, fpb, bpa; // forward and backward

	if (size_a != a.GetSize() || size_b != b.GetSize())
	{
		size_a = a.GetSize(); size_b = b.GetSize();
		fftw_destroy_plan(fpa); fftw_destroy_plan(bpa); fftw_destroy_plan(fpb); //fftw_destroy_plan(bpb);
		if (nA) fftw_free(nA);
		if (nB) fftw_free(nB);
		if (fA) fftw_free(fA);
		if (fB) fftw_free(fB);

		nA = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size_a);
		nB = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size_b);
		fA = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size_a);
		fB = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size_b);

		fpa = fftw_plan_dft_1d(size_a, nA, fA, FFTW_FORWARD, FFTW_ESTIMATE);
		fpb = fftw_plan_dft_1d(size_b, nB, fB, FFTW_FORWARD, FFTW_ESTIMATE);
		bpa = fftw_plan_dft_1d(size_a, fA, nA, FFTW_BACKWARD, FFTW_ESTIMATE);
		//bpb = fftw_plan_dft_1d(size_b, fB, nB, FFTW_BACKWARD, FFTW_ESTIMATE);
	}

	for (uint i = 0; i < a.GetSize(); i++)
	{
		nA[i][0] = a.GetData()[i];
		nA[i][1] = 0;
	}
	fftw_execute(fpa);

	for (uint i = 0; i < b.GetSize(); i++)
	{
		nB[i][0] = b.GetData()[i];
		nB[i][1] = 0;
	}
	fftw_execute(fpb);

	for (uint i = 0; i < a.GetSize(); i++) // выход за пределы, если массивы разного размера. Но этого не должно случиться.
	{
		auto tmp_compl = std::complex<num>(fA[i][0], fA[i][1]) * std::complex<num>(fB[i][0], fB[i][1]);
		fA[i][0] = tmp_compl.real();
		fA[i][1] = tmp_compl.imag();
	}

	fftw_execute(bpa);

	Distribution1D res = a; // TODO: проверить копирование. Лучше сделать через прямое создание
	for (uint i = 0; i < res.GetSize(); i++)
	{
		res.GetData()[i] = nA[i][0];
	}

	return res;
}
#endif // FFTW