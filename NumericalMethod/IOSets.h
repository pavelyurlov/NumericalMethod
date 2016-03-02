#pragma once

#include "minitypes.h"
#include "MatlabVector.h"
#include "json\json.h"
#include <iostream>

struct InputSet
{
	num A;
	uint N;
	uint max_iter;
	num a;
	num(*f_w11)(num);
	num(*f_w12)(num);
	num(*f_w21)(num);
	num(*f_w22)(num);
	num(*f_m1)(num);
	num(*f_m2)(num);
	num b1;
	num b2;
	num d1;
	num d2;
	num d11;
	num d12;
	num d21;
	num d22;
	operator Json::Value() const;
};

struct OutputSet
{
	MatlabVector rh, D11, D12, D22;
	num y11, y12, y21, y22, N1, N2;
	operator Json::Value() const;
};

std::ostream& operator<<(std::ostream&, OutputSet&);
std::ostream& operator<<(std::ostream&, InputSet&);