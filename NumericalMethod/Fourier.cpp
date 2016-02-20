#define _USE_MATH_DEFINES
#include <math.h>
#include "MatlabVector.h"
#include <complex>
//#include <fftw3.h> // Win
//#include <fftw/fftw3.h> // Lom

#include <mkl_types.h>
#include <mkl.h>
#include <mkl_dfti.h>


MatlabVector conv_fourier_lib(MatlabVector a, MatlabVector b)
{
	static uint size_a = 0, size_b = 0;
	//static fftw_complex *nA, *nB, *fA, *fB; // n - normal, f - transformed
	static MKL_Complex16 *nA, *nB, *fA, *fB;
	//static fftw_plan fpa, fpb, bpa; // forward and backward
	static DFTI_DESCRIPTOR_HANDLE fpa, fpb, bpa;

	if (size_a != a.size() || size_b != b.size())
	{
		size_a = a.size(); size_b = b.size();
		//fftw_destroy_plan(fpa); fftw_destroy_plan(bpa); fftw_destroy_plan(fpb); //fftw_destroy_plan(bpb);
		DftiFreeDescriptor(&fpa); DftiFreeDescriptor(&bpa); DftiFreeDescriptor(&fpb);
		/*if (nA) fftw_free(nA);
		if (nB) fftw_free(nB);
		if (fA) fftw_free(fA);
		if (fB) fftw_free(fB);*/
		if (nA) mkl_free(nA);
		if (nB) mkl_free(nB);
		if (fA) mkl_free(fA);
		if (fB) mkl_free(fB);

		/*nA = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size_a);
		nB = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size_b);
		fA = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size_a);
		fB = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size_b);*/
		nA = (MKL_Complex16*)mkl_malloc(sizeof(MKL_Complex16) * size_a, 0);
		nB = (MKL_Complex16*)mkl_malloc(sizeof(MKL_Complex16) * size_b, 0);
		fA = (MKL_Complex16*)mkl_malloc(sizeof(MKL_Complex16) * size_a, 0);
		fB = (MKL_Complex16*)mkl_malloc(sizeof(MKL_Complex16) * size_b, 0);

		/*fpa = fftw_plan_dft_1d(size_a, nA, fA, FFTW_FORWARD, FFTW_ESTIMATE);
		fpb = fftw_plan_dft_1d(size_b, nB, fB, FFTW_FORWARD, FFTW_ESTIMATE);
		bpa = fftw_plan_dft_1d(size_a, fA, nA, FFTW_BACKWARD, FFTW_ESTIMATE);*/
		DftiCreateDescriptor(&fpa, DFTI_DOUBLE, DFTI_COMPLEX, 1, size_a);
		DftiCreateDescriptor(&fpb, DFTI_DOUBLE, DFTI_COMPLEX, 1, size_b);
		DftiCreateDescriptor(&bpa, DFTI_DOUBLE, DFTI_COMPLEX, 1, size_a);
		DftiCommitDescriptor(fpa);
		DftiCommitDescriptor(fpb);
		DftiCommitDescriptor(bpa);
	}

	for (uint i = 0; i < a.size(); i++)
	{
		//nA[i][0] = a[i];
		//nA[i][1] = 0;
		nA[i].real = a[i];
		nA[i].imag = 0;
	}
	//fftw_execute(fpa);
	DftiComputeForward(fpa, nA, fA);

	for (uint i = 0; i < b.size(); i++)
	{
		//nB[i][0] = b[i];
		//nB[i][1] = 0;
		nB[i].real = b[i];
		nB[i].imag = 0;
	}
	//fftw_execute(fpb);
	DftiComputeForward(fpb, nB, fB);

	if (a.size() != b.size()) throw new char*("a.size != b.size");
	for (uint i = 0; i < a.size(); i++) // выход за пределы, если массивы разного размера. Но этого не должно случиться.
	{
		//auto tmp_compl = std::complex<num>(fA[i][0], fA[i][1]) * std::complex<num>(fB[i][0], fB[i][1]);
		//fA[i][0] = tmp_compl.real();
		//fA[i][1] = tmp_compl.imag();
		auto tmp_compl = std::complex<num>(fA[i].real, fA[i].imag) * std::complex<num>(fB[i].real, fB[i].imag);
		fA[i].real = tmp_compl.real();
		fA[i].imag = tmp_compl.imag();
	}

	//fftw_execute(bpa);
	DftiComputeBackward(bpa, fA, nA);

	MatlabVector res = MatlabVector(a.size());
	for (uint i = 0; i < a.size(); i++)
	{
		a[i] = nA[i].real;
	}

	return res;
}

MatlabVector conv(MatlabVector a, MatlabVector b, int) // int в конце несущественнен
{
	return conv_fourier_lib(a, b);
}