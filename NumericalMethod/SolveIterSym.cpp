// Almost literal port from "solve_iter_sym.m".

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstring>
#include <limits>
#include <numeric>
#include <iostream>
#include "minitypes.h"
#include "MatlabVector.h"
#include "RadialDistribution.h"
#include "IOSets.h"
#include "normpdf_checked.h"

MatlabVector precount_func(num param, num sigma, uint N, MatlabVector &rh);
RadialDistribution conv(RadialDistribution a, RadialDistribution b);
extern OutputSet solve_iter_sym_one_kind(num A, uint N, uint max_iter, num a, num sw11, num sw12, num sw21, num sw22, num sm1, num sm2, num b1, num b2, num d1, num d2, num d11, num d12, num d21, num d22);

MatlabVector g_rh;
num g_A;
uint g_N;

// TODO: превратить SolveIterSym в полноценный класс
static InputSet gs_is;

num count_integral(MatlabVector a, MatlabVector b, num h)
{
	MatlabVector tmp = a*b;
	num res = h * std::accumulate(tmp.begin(), tmp.end(), 0.0);
	return res;
}

num precount_func(num param, num sigma, uint N, num r) { return param * normpdf_checked(r, sigma, 1); }

num m1_init(num r) { return precount_func(gs_is.b1, gs_is.sm1, gs_is.N, r); }
num m2_init(num r) { return precount_func(gs_is.b2, gs_is.sm2, gs_is.N, r); }
num w11_init(num r) { return precount_func(gs_is.d11, gs_is.sw11, gs_is.N, r); }
num w12_init(num r) { return precount_func(gs_is.d12, gs_is.sw12, gs_is.N, r); }
num w21_init(num r) { return precount_func(gs_is.d21, gs_is.sw21, gs_is.N, r); }
num w22_init(num r) { return precount_func(gs_is.d22, gs_is.sw22, gs_is.N, r); }

#define COLLECT_RESULTS_MAKROS result.D11 = D11; result.D12 = D12; result.D22 = D22; \
result.N1 = N1; result.N2 = N2; \
result.y11 = y11; result.y12 = y12; result.y21 = y21; result.y22 = y22

OutputSet solve_iter_sym(num A, uint N, uint max_iter, num a, num sw11, num sw12, num sw21, num sw22, num sm1, num sm2, num b1, num b2, num d1, num d2, num d11, num d12, num d21, num d22)
{
	OutputSet result(preferences.dimentions, N, A);

	// precount functions
	g_A = A;
	g_N = N;
	num h = 2 * A / N;
	if (preferences.dimentions == 2) h = h*h;
	if (preferences.dimentions == 3) h = h*h*h;

	RadialDistribution m1 (preferences.dimentions, N, A, m1_init);
	RadialDistribution m2 (preferences.dimentions, N, A, m2_init);
	RadialDistribution w11(preferences.dimentions, N, A, w11_init);
	RadialDistribution w12(preferences.dimentions, N, A, w12_init);
	RadialDistribution w21(preferences.dimentions, N, A, w21_init);
	RadialDistribution w22(preferences.dimentions, N, A, w22_init);

	// init
	RadialDistribution D11(preferences.dimentions, N, A);
	RadialDistribution D12(preferences.dimentions, N, A);
	RadialDistribution D22(preferences.dimentions, N, A);

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
		COLLECT_RESULTS_MAKROS;
		//std::cout << result;

		RadialDistribution first = h * conv((m1 + m2), D12) - w21 - w12 -
			((a / 2)*N1)*(h * (D12 + 2) * (conv(w11, D12) + conv(w21, D11)) +
				h * conv(D11, w21*D12) + h * conv(D12, w11*D11)) -
			(a / 2)*N2*(h * (D12 + 2) * (conv(w12, D22) + conv(w22, D12)) +
				h * conv(D12, w22*D22) + h * conv(D22, w12*D12));
		RadialDistribution second = w12 + w21 + (1 - a / 2)*(b1 + b2) + (a / 2)*(d1 + d2 + d11*N1 + d12*N2 + d21*N1 + d22*N2);
		D12 = first / second;
		// 61 секунда при свёртке в лоб
		// 549 секунд при свёртке с дискретным преобразованием Фурье (думаю, из-за экспонент)
		// 249 секунд при свёртке с дискретным преобразованием Фурье с неоптимальным кешированием (154 МБ памяти -> лишение кеша процессора)
		// 0,25 секунд с библиотекой fftw
		// мораль очевидна :D

		y12 = (w12 * D12).CountIntegral() + d12;
		y21 = (w21 * D12).CountIntegral() + d21;

		if (isnan(y12) || isnan(y21))
		{
			y11 = NAN;
			y12 = NAN;
			y21 = NAN;
			y22 = NAN;
			COLLECT_RESULTS_MAKROS;
			return result;
		}

		N1 = ((b1 - d1)*y22 - (b2 - d2)*y12) / (y11*y22 - y12*y21);
		N2 = ((b2 - d2)*y11 - (b1 - d1)*y21) / (y11*y22 - y12*y21);

		first = (1 / N1) * m1 - w11 + h * conv(m1, D11) -
			(a / 2)*N1*(h * (D11 + 2)*conv(w11, D11) + h * conv(D11, w11*D11)) -
			(a / 2)*N2*(h * (D11 + 2)*conv(w12, D12) + h * conv(D12, w12*D12));
		second = w11 + (1 - a / 2)*b1 + (a / 2)*(d1 + N1*d11 + N2*d12);
		D11 = first / second;

		first = (1 / N2) * m2 - w22 + h * conv(m2, D22) -
			(a / 2)*N2*(h * (D22 + 2)*conv(w22, D22) + h * conv(D22, w22*D22)) -
			(a / 2)*N1*(h * (D22 + 2)*conv(w21, D12) + h * conv(D12, w21*D12));
		second = w22 + (1 - a / 2)*b2 + (a / 2)*(d2 + N2*d22 + N1*d21);
		D22 = first / second;

		y11 = (w11 * D11).CountIntegral() + d11;
		y22 = (w22 * D22).CountIntegral() + d22;

		if (isnan(y11) || isnan(y11))
		{
			y11 = NAN;
			y12 = NAN;
			y21 = NAN;
			y22 = NAN;
			COLLECT_RESULTS_MAKROS;
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
			COLLECT_RESULTS_MAKROS;
			return result;
		}

		prev_err = err;
		N1 = N1_new;
		N2 = N2_new;

		g_iterations_count++;
	}
	COLLECT_RESULTS_MAKROS;
	return result;
}

OutputSet solve_iter_sym(InputSet s)
{
	gs_is = s;
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
	uint number = rh.size();
	MatlabVector result = MatlabVector(number);
	for (uint i = 0; i < number; i++)
	{
		result[i] = param * normpdf_checked(rh[i], sigma, 1);
	}
	return result;
}
