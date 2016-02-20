#pragma once

#include <vector>
#include "minitypes.h"

class MatlabVector :
	public std::vector<num>
{
public:
	MatlabVector() {}
	MatlabVector(uint N) :std::vector<num>(N) {} // garbage inside?
	template <class InputIterator> MatlabVector(InputIterator first, InputIterator last) : vector<num>(first, last) {}
	MatlabVector operator_plus(MatlabVector&o) { return per_member_op(o, [](num a, num b) {return a + b; }); } // matlab .+
	MatlabVector operator_minus(MatlabVector&o) { return per_member_op(o, [](num a, num b) {return a - b; }); }
	MatlabVector operator_frag(MatlabVector&o) { return per_member_op(o, [](num a, num b) {return a / b; }); }
	MatlabVector operator_mult(MatlabVector&o) { return per_member_op(o, [](num a, num b) {return a * b; }); }
	MatlabVector operator+(num a) { return per_member_op(a, [](num b, num c) {return b + c; }); }
	MatlabVector operator*(num a) { return per_member_op(a, [](num b, num c) {return b * c; }); }
private:
	MatlabVector per_member_op(MatlabVector&, num(*op)(num, num));
	MatlabVector per_member_op(num, num(*op)(num, num));
};

MatlabVector operator*(num n, MatlabVector mv);

MatlabVector operator+(MatlabVector a, MatlabVector b);// { return a.operator_plus(b); }
MatlabVector operator*(MatlabVector a, MatlabVector b);// { return a.operator_mult(b); }
MatlabVector operator-(MatlabVector a, MatlabVector b);// { return a.operator_minus(b); }
MatlabVector operator/(MatlabVector a, MatlabVector b);// { return a.operator_frag(b); }

