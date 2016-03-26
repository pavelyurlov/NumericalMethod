// Almost literal port from "solve_iter_sym.m".

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstring>
#include <limits>
#include <numeric>
#include <iostream>
#include "minitypes.h"
#include "MatlabVector.h"
#include "IOSets.h"
#include "normpdf_checked.h"

MatlabVector linspace(num start, num end, uint n_points);
MatlabVector precount_func(num param, num sigma, uint N, MatlabVector &rh);
MatlabVector conv(MatlabVector a, MatlabVector b, int);
extern OutputSet solve_iter_sym_one_kind(num A, uint N, uint max_iter, num a, num sw11, num sw12, num sw21, num sw22, num sm1, num sm2, num b1, num b2, num d1, num d2, num d11, num d12, num d21, num d22);


// надо убрать повторяющийся блок кода в collecting results. Макросы?
OutputSet solve_iter_sym(num A, uint N, uint max_iter, num a, num sw11, num sw12, num sw21, num sw22, num sm1, num sm2, num b1, num b2, num d1, num d2, num d11, num d12, num d21, num d22)
{
	OutputSet result;

	// precount functions
	MatlabVector rh = linspace(-A, A, N);
	num h = rh[2] - rh[1];

	MatlabVector m1 = precount_func(b1, sm1, N, rh);
	MatlabVector m2 = precount_func(b2, sm2, N, rh);
	MatlabVector w11 = precount_func(d11, sw11, N, rh);
	MatlabVector w21 = precount_func(d21, sw21, N, rh);
	MatlabVector w12 = precount_func(d12, sw12, N, rh);
	MatlabVector w22 = precount_func(d22, sw22, N, rh);

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
		//std::cout << result;

		MatlabVector test = m1 * m2;

		MatlabVector first = h * conv((m1 + m2), D12, 'same') - w21 - w12 -
			((a / 2)*N1)*(h * (D12 + 2) * (conv(w11, D12, 'same') + conv(w21, D11, 'same')) +
				h * conv(D11, w21*D12, 'same') + h * conv(D12, w11*D11, 'same')) -
			(a / 2)*N2*(h * (D12 + 2) * (conv(w12, D22, 'same') + conv(w22, D12, 'same')) +
				h * conv(D12, w22*D22, 'same') + h * conv(D22, w12*D12, 'same'));
		MatlabVector second = w12 + w21 + (1 - a / 2)*(b1 + b2) + (a / 2)*(d1 + d2 + d11*N1 + d12*N2 + d21*N1 + d22*N2);
		D12 = first / second;
		// 61 секунда при свёртке в лоб
		// 549 секунд при свёртке с дискретным преобразованием Фурье (думаю, из-за экспонент)
		// 249 секунд при свёртке с дискретным преобразованием Фурье с неоптимальным кешированием (154 МБ памяти -> лишение кеша процессора)
		// 0,25 секунд с библиотекой fftw
		// мораль очевидна :D

		MatlabVector tmp = w12*D12;
		y12 = h * std::accumulate(tmp.begin(), tmp.end(), 0.0) + d12;

		tmp = w21*D12;
		y21 = h * std::accumulate(tmp.begin(), tmp.end(), 0.0) + d21;

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

		tmp = w11*D11;
		y11 = h * std::accumulate(tmp.begin(), tmp.end(), 0.0) + d11;
		tmp = w22*D22;
		y22 = h * std::accumulate(tmp.begin(), tmp.end(), 0.0) + d22;

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
	if (preferences.one_kind)
		return solve_iter_sym_one_kind(s.A, s.N, s.max_iter, s.a, s.sw11, s.sw12, s.sw21, s.sw22, s.sm1, s.sm2, s.b1, s.b2, s.d1, s.d2, s.d11, s.d12, s.d21, s.d22);
	else
		return solve_iter_sym(s.A, s.N, s.max_iter, s.a, s.sw11, s.sw12, s.sw21, s.sw22, s.sm1, s.sm2, s.b1, s.b2, s.d1, s.d2, s.d11, s.d12, s.d21, s.d22);
}

template <typename T>
void swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

MatlabVector precount_func(num param, num sigma, uint N, MatlabVector &rh)
{
	MatlabVector result = MatlabVector(N);
	for (uint i = 0; i < N; i++)
	{
		result[i] = param * normpdf_checked(rh[i], sigma, 1);
	}
	return result;
}

MatlabVector linspace(num start, num end, uint n_points)
{
	if (start > end) swap(start, end);
	num step = (end - start) / n_points;
	MatlabVector result = MatlabVector(n_points);
	for (uint i = 0; i < n_points; i++)
	{
		result[i] = start + step * i;
	}
	return result;
}
