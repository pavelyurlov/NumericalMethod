#pragma once

#include <vector>
#include "minitypes.h"

class MatlabVector :
	public std::vector<num>
{
public:
	MatlabVector() {}
	MatlabVector(uint N) :vector<num>(N) {} // garbage inside?
	template <class InputIterator> MatlabVector(InputIterator first, InputIterator last) : vector<num>(first, last) {}
	MatlabVector operator+(MatlabVector&o) { return per_member_op(o, [](num a, num b) {return a + b; }); } // matlab .+
	MatlabVector operator-(MatlabVector&o) { return per_member_op(o, [](num a, num b) {return a - b; }); }
	MatlabVector operator/(MatlabVector&o) { return per_member_op(o, [](num a, num b) {return a / b; }); }
	MatlabVector operator*(MatlabVector&o) { return per_member_op(o, [](num a, num b) {return a * b; }); }
	MatlabVector operator+(num a) { return per_member_op(a, [](num b, num c) {return b + c; }); }
	MatlabVector operator*(num a) { return per_member_op(a, [](num b, num c) {return b * c; }); }
private:
	MatlabVector per_member_op(MatlabVector&, num(*op)(num, num));
	MatlabVector per_member_op(num, num(*op)(num, num));
};

MatlabVector operator*(num n, MatlabVector mv);