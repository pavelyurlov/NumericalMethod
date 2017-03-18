#include "NumericalMethod.h"
#include "Distribution1D.h"
#include "Partition.h"
#include "Fourier.h"

NumericalMethod::NumericalMethod(InputSet input):
	c_input(input), m_part(Partition::GetPartition(input.A, input.N)), m_result(Partition::GetPartition(input.A, input.N)) {}

OutputSet NumericalMethod::GetResult()
{
	return m_result;
}

void NumericalMethod::Count()
{
	// syntax sugar
	const InputSet &s = c_input;	
	OutputSet &o = m_result;		
	Partition &p = m_part;
	const num h = m_part.GetStep();
	Distribution1D(&conv)(Distribution1D, Distribution1D) = Fourier::Convolution;

	// init
	Distribution1D m1(p,  NormalFunction1D(s.b1,  s.sm1));
	Distribution1D m2(p,  NormalFunction1D(s.b2,  s.sm2));
	Distribution1D w11(p, NormalFunction1D(s.d11, s.sw11));
	Distribution1D w21(p, NormalFunction1D(s.d21, s.sw21));
	Distribution1D w12(p, NormalFunction1D(s.d12, s.sw12));
	Distribution1D w22(p, NormalFunction1D(s.d22, s.sw22));

	o.y11 = s.d11;
	o.y12 = s.d12;
	o.y21 = s.d21;
	o.y22 = s.d22;
	o.N1 = 0;
	o.N2 = 0;

	num prev_err = std::numeric_limits<num>::max();
	num eps = static_cast<num>(1e-2);

	// count
	for (uint i = 0; i < Preferences::max_iter; i++) // TODO: max_iter - в InputSet
	{
		Distribution1D first = h * conv((m1 + m2), o.D12) - w21 - w12 -
			((s.a / 2)*o.N1)*(h * (o.D12 + 2) * (conv(w11, o.D12) + conv(w21, o.D11)) +
				h * conv(o.D11, w21*o.D12) + h * conv(o.D12, w11*o.D11)) -
			(s.a / 2)*o.N2*(h * (o.D12 + 2) * (conv(w12, o.D22) + conv(w22, o.D12)) +
				h * conv(o.D12, w22*o.D22) + h * conv(o.D22, w12*o.D12));
		Distribution1D second = w12 + w21 + (1 - s.a / 2)*(s.b1 + s.b2) + (s.a / 2)*(s.d1 + s.d2 + s.d11*o.N1 + o.D12*o.N2 + s.d21*o.N1 + o.D22*o.N2);
		o.D12 = first / second;

		o.y12 = (w12 * o.D12).CountIntegral() + s.d12;
		o.y21 = (w21 * o.D12).CountIntegral() + s.d21;

		if (isnan(o.y12) || isnan(o.y21)) // TODO: по хорошему, надо такие случаи флагами обрабатывать
		{
			o.y11 = NAN;
			o.y12 = NAN;
			o.y21 = NAN;
			o.y22 = NAN;
			return;
		}

		o.N1 = ((s.b1 - s.d1)*o.y22 - (s.b2 - s.d2)*o.y12) / (o.y11*o.y22 - o.y12*o.y21);
		o.N2 = ((s.b2 - s.d2)*o.y11 - (s.b1 - s.d1)*o.y21) / (o.y11*o.y22 - o.y12*o.y21);

		first = (1 / o.N1) * m1 - w11 + h * conv(m1, o.D11) -
			(s.a / 2)*o.N1*(h * (o.D11 + 2)*conv(w11, o.D11) + h * conv(o.D11, w11*o.D11)) -
			(s.a / 2)*o.N2*(h * (o.D11 + 2)*conv(w12, o.D12) + h * conv(o.D12, w12*o.D12));
		second = w11 + (1 - s.a / 2)*s.b1 + (s.a / 2)*(s.d1 + o.N1*o.D11 + o.N2*o.D12);
		o.D11 = first / second;

		first = (1 / o.N2) * m2 - w22 + h * conv(m2, o.D22) -
			(s.a / 2)*o.N2*(h * (o.D22 + 2)*conv(w22, o.D22) + h * conv(o.D22, w22*o.D22)) -
			(s.a / 2)*o.N1*(h * (o.D22 + 2)*conv(w21, o.D12) + h * conv(o.D12, w21*o.D12));
		second = w22 + (1 - s.a / 2)*s.b2 + (s.a / 2)*(s.d2 + o.N2*o.D22 + o.N1*o.D12);
		o.D22 = first / second;

		o.y11 = (w11 * o.D11).CountIntegral() + s.d11;
		o.y22 = (w22 * o.D22).CountIntegral() + s.d22;

		if (isnan(o.y11) || isnan(o.y22))
		{
			o.y11 = NAN;
			o.y12 = NAN;
			o.y21 = NAN;
			o.y22 = NAN;
			return;
		}

		num N1_new = ((s.b1 - s.d1)*o.y22 - (s.b2 - s.d2)*o.y12) / (o.y11*o.y22 - o.y12*o.y21);
		num N2_new = ((s.b2 - s.d2)*o.y11 - (s.b1 - s.d1)*o.y21) / (o.y11*o.y22 - o.y12*o.y21);

		num err = abs(o.N2 - N2_new) + abs(o.N1 - N1_new);
		if ((err < eps) || (err > prev_err))
		{
			if (err > prev_err)
			{
				o.y11 = NAN;
				o.y12 = NAN;
				o.y21 = NAN;
				o.y22 = NAN;
			}
			return;
		}

		prev_err = err;
		o.N1 = N1_new;
		o.N2 = N2_new;

		g_iterations_count++;
	}
}
