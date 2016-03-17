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

		N1 = (b1 - d1) / y11;

		MatlabVector first = (1 / N1) * m1 - w11 + h * conv(m1, D11, 'same') -
			(a / 2)*N1*(h * (D11 + 2)*conv(w11, D11, 'same') + h * conv(D11, w11*D11, 'same'));
		MatlabVector second = w11 + (1 - a / 2)*b1 + (a / 2)*(d1 + N1*d11);
		D11 = first / second;

		MatlabVector tmp = w11*D11;
		y11 = h * std::accumulate(tmp.begin(), tmp.end(), 0.0) + d11;
		
		if (isnan(y11) || isnan(y11)) // wtf?
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

		num N1_new = (b1 - d1) / y11;

		num err = abs(N1 - N1_new);
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
