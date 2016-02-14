// Almost literal port from "solve_iter_sym.m".

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstring>
#include <limits>
#include <numeric>
#include <iostream>
#include <complex>
#include "minitypes.h"
#include "MatlabVector.h"
#include "IOSets.h"
#include "ERROR.h"

MatlabVector& linspace(num start, num end, uint n_points);
MatlabVector& precount_func(num param, num(*f)(num), uint N, MatlabVector &rh);
MatlabVector& conv(MatlabVector a, MatlabVector b, int);


// надо убрать повторяющийся блок кода в collecting results. Макросы?
OutputSet solve_iter_sym(num A, uint N, uint max_iter, num a, num(*f_w11)(num), num(*f_w12)(num), num(*f_w21)(num), num(*f_w22)(num), num(*f_m1)(num), num(*f_m2)(num), num b1, num b2, num d1, num d2, num d11, num d12, num d21, num d22)
{
	OutputSet result;

	// precount functions
	MatlabVector rh = linspace(-A, A, N);
	num h = rh[2] - rh[1];

	MatlabVector m1 = precount_func(b1, f_m1, N, rh);
	MatlabVector m2 = precount_func(b2, f_m2, N, rh);
	MatlabVector w11 = precount_func(d11, f_w11, N, rh);
	MatlabVector w21 = precount_func(d21, f_w21, N, rh);
	MatlabVector w12 = precount_func(d12, f_w12, N, rh);
	MatlabVector w22 = precount_func(d22, f_w22, N, rh);

	// init
	MatlabVector D11 = MatlabVector(N); std::fill(D11.begin(), D11.end(), 0);
	MatlabVector D12 = MatlabVector(N); std::fill(D12.begin(), D12.end(), 0);
	MatlabVector D22 = MatlabVector(N); std::fill(D22.begin(), D22.end(), 0);

	num y11 = d11;
	num y12 = d12;
	num y21 = d21;
	num y22 = d22;
	num N1 = 0;
	num N2 = 0;

	num prev_err = std::numeric_limits<num>::max();
	num eps = static_cast<num>(1e-2);

	// count
	for (uint i = 0; i < max_iter; i++)
	{
		// collect results
		result.D11 = D11; result.D12 = D12; result.D22 = D22;
		result.N1 = N1; result.N2 = N2; result.rh = rh;
		result.y11 = y11; result.y12 = y12; result.y21 = y21; result.y22 = y22;
		// end collect results
		std::cout << result;

		MatlabVector first = h * conv((m1 + m2), D12, 'same') - w21 - w12 -
			((a / 2)*N1)*(h * (D12 + 2) * (conv(w11, D12, 'same') + conv(w21, D11, 'same')) +
				h * conv(D11, w21*D12, 'same') + h * conv(D12, w11*D11, 'same')) -
			(a / 2)*N2*(h * (D12 + 2) * (conv(w12, D22, 'same') + conv(w22, D12, 'same')) +
				h * conv(D12, w22*D22, 'same') + h * conv(D22, w12*D12, 'same'));
		MatlabVector second = w12 + w21 + (1 - a / 2)*(b1 + b2) + (a / 2)*(d1 + d2 + d11*N1 + d12*N2 + d21*N1 + d22*N2);
		D12 = first / second;
		// 61 секунда при свёртке в лоб
		// 549 секунд при свёртке с дискретным преобразованием Фурье (думаю, из-за экспонент)

		MatlabVector *tmp = &(w12*D12);
		y12 = h * std::accumulate(tmp->begin(), tmp->end(), 0.0) + d12;

		tmp = &(w21*D12);
		y21 = h * std::accumulate(tmp->begin(), tmp->end(), 0.0) + d21;

		if (isnan(y12) || isnan(y21))
		{
			y11 = NAN;
			y12 = NAN;
			y21 = NAN;
			y22 = NAN;
			// collect results
			result.D11 = D11; result.D12 = D12; result.D22 = D22;
			result.N1 = N1; result.N2 = N2; result.rh = rh;
			result.y11 = y11; result.y12 = y12; result.y21 = y21; result.y22 = y22;
			// end collect results
			return result;
		}

		N1 = ((b1 - d1)*y22 - (b2 - d2)*y12) / (y11*y22 - y12*y21);
		N2 = ((b2 - d2)*y11 - (b1 - d1)*y21) / (y11*y22 - y12*y21);

		first = (1 / N1) * m1 - w11 + h * conv(m1, D11, 'same') -
			(a / 2)*N1*(h * (D11 + 2)*conv(w11, D11, 'same') + h * conv(D11, w11*D11, 'same')) -
			(a / 2)*N2*(h * (D11 + 2)*conv(w12, D12, 'same') + h * conv(D12, w12*D12, 'same'));
		second = w11 + (1 - a / 2)*b1 + (a / 2)*(d1 + N1*d11 + N2*d12);
		D11 = first / second;

		first = (1 / N2) * m2 - w22 + h * conv(m2, D22, 'same') -
			(a / 2)*N2*(h * (D22 + 2)*conv(w22, D22, 'same') + h * conv(D22, w22*D22, 'same')) -
			(a / 2)*N1*(h * (D22 + 2)*conv(w21, D12, 'same') + h * conv(D12, w21*D12, 'same'));
		second = w22 + (1 - a / 2)*b2 + (a / 2)*(d2 + N2*d22 + N1*d21);
		D22 = first / second;

		tmp = &(w11*D11);
		y11 = h * std::accumulate(tmp->begin(), tmp->end(), 0.0) + d11;
		tmp = &(w22*D22);
		y22 = h * std::accumulate(tmp->begin(), tmp->end(), 0.0) + d22;

		if (isnan(y11) || isnan(y11))
		{
			y11 = NAN;
			y12 = NAN;
			y21 = NAN;
			y22 = NAN;
			// collect results
			result.D11 = D11; result.D12 = D12; result.D22 = D22;
			result.N1 = N1; result.N2 = N2; result.rh = rh;
			result.y11 = y11; result.y12 = y12; result.y21 = y21; result.y22 = y22;
			// end collect results
			return result;
		}

		num N1_new = ((b1 - d1)*y22 - (b2 - d2)*y12) / (y11*y22 - y12*y21);
		num N2_new = ((b2 - d2)*y11 - (b1 - d1)*y21) / (y11*y22 - y12*y21);

		num err = abs(N2 - N2_new) + abs(N1 - N1_new);
		if ((err < eps) || (err > prev_err))
		{
			if (err > prev_err)
			{
				y11 = NAN;
				y12 = NAN;
				y21 = NAN;
				y22 = NAN;
			}
			// collect results
			result.D11 = D11; result.D12 = D12; result.D22 = D22;
			result.N1 = N1; result.N2 = N2; result.rh = rh;
			result.y11 = y11; result.y12 = y12; result.y21 = y21; result.y22 = y22;
			// end collect results
			return result;
		}

		prev_err = err;
		N1 = N1_new;
		N2 = N2_new;
	}
	// collect results
	result.D11 = D11; result.D12 = D12; result.D22 = D22;
	result.N1 = N1; result.N2 = N2; result.rh = rh;
	result.y11 = y11; result.y12 = y12; result.y21 = y21; result.y22 = y22;
	// end collect results
	return result;
}

OutputSet solve_iter_sym(InputSet s)
{
	return solve_iter_sym(s.A, s.N, s.max_iter, s.a, s.f_w11, s.f_w12, s.f_w21, s.f_w22, s.f_m1, s.f_m2, s.b1, s.b2, s.d1, s.d2, s.d11, s.d12, s.d21, s.d22);
}

template <typename T>
void swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

MatlabVector& precount_func(num param, num(*f)(num), uint N, MatlabVector &rh)
{
	MatlabVector &result = *(new MatlabVector(N)); // память?
	for (uint i = 0; i < N; i++)
	{
		result[i] = param * f(rh[i]);
	}
	return result;
}

MatlabVector& linspace(num start, num end, uint n_points)
{
	if (start > end) swap(start, end);
	num step = (end - start) / n_points;
	MatlabVector &result = *(new MatlabVector(n_points)); // может быть утечка памяти
	for (uint i = 0; i < n_points; i++)
	{
		result[i] = start + step * i;
	}
	return result;
}

// http://stackoverflow.com/questions/8424170/1d-linear-convolution-in-ansi-c-code
MatlabVector& conv_direct(MatlabVector a, MatlabVector b)
{
	MatlabVector &result = *(new MatlabVector(a.size())); // опять-таки, может утечь память

	for (uint i = 0 + b.size() / 2; i < a.size() + b.size() / 2 - 1; i++)
	{
		result[i - b.size() / 2] = 0; // только результат. т.е у результата всё сдвинуто влево. У остальных всё нормально.
		uint start = (i >= b.size() - 1) ? i - (b.size() - 1) : 0;
		uint end = (i < a.size() - 1) ? i : a.size() - 1;
		for (uint j = start; j <= end; j++)
		{
			result[i - b.size() / 2] += a[j] * b[i - j];
		}
	}

	return result;
}

std::vector<std::complex<num>>& dft(MatlabVector &x) // прямое дискретное преобразование Фурье
{
	std::vector<std::complex<num>> &res = *(new std::vector<std::complex<num>>(x.size()));
	for (uint i = 0; i < res.size(); i++)
	{
		res[i] = 0;
		for (uint j = 0; j < x.size(); j++)
		{
			res[i] += x[j] * exp(
				(-std::complex<num>(2 * M_PI *i * j, 0) * std::complex<num>(0, 1))
				/
				(std::complex<num>(x.size())));
		}
	}
	return res;
}

MatlabVector& idft(std::vector<std::complex<num>> &x) // обратное дискретное преобразование Фурье
{
	MatlabVector &res = *(new MatlabVector(x.size()));
	for (uint i = 0; i < res.size(); i++)
	{
		std::complex<num> compl_res = 0;
		for (uint j = 0; j < x.size(); j++)
		{
			compl_res += x[j] * exp(
				(std::complex<num>(2 * M_PI *i * j, 0) * std::complex<num>(0, 1))
				/
				(std::complex<num>(x.size())));
		}
		res[i] = compl_res.real() / static_cast<num>(x.size());
	}
	return res;
}

MatlabVector& conv_fourier(MatlabVector a, MatlabVector b)
{
	auto A = dft(a);
	auto B = dft(b);
	for (uint i = 0; i < A.size(); i++)
	{
		A[i] = A[i] * B[i];
	}
	return idft(A);
}

MatlabVector& conv(MatlabVector a, MatlabVector b, int) // int в конце несущественнен
{
	return conv_direct(a, b);
}