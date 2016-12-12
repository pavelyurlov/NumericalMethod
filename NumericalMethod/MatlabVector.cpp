#include "MatlabVector.h"
#include <string>


MatlabVector MatlabVector::per_member_op(MatlabVector &other, num(*op)(num, num))
{
	if (this->size() != other.size()) throw new std::string("почленна€ операци€ над векторами разного размера недопустима");
	MatlabVector result = MatlabVector(this->size());
	for (uint i = 0; i < this->size(); i++) result[i] = op((*this)[i], other[i]);
	return result;
}

MatlabVector MatlabVector::per_member_op(num a, num(*op)(num, num))
{
	MatlabVector result = MatlabVector(this->size());
	for (uint i = 0; i < this->size(); i++) result[i] = op((*this)[i], a);
	return result;
}

//ќк, почему эти операторы летают в воздухе? –азве не должно быть MatlabVector::operator?
MatlabVector operator*(num n, MatlabVector mv) { return mv*n; }

MatlabVector operator+(MatlabVector a, MatlabVector b)
{
	if (a.size() != b.size()) throw new std::string("почленна€ операци€ над векторами разного размера недопустима");
	MatlabVector result = MatlabVector(a.size());
	for (uint i = 0; i < a.size(); i++) result[i] = a[i] + b[i];
	return result;
}

MatlabVector operator-(MatlabVector a, MatlabVector b)
{
	if (a.size() != b.size()) throw new std::string("почленна€ операци€ над векторами разного размера недопустима");
	MatlabVector result = MatlabVector(a.size());
	for (uint i = 0; i < a.size(); i++) result[i] = a[i] - b[i];
	return result;
}

MatlabVector operator*(MatlabVector a, MatlabVector b)
{
	if (a.size() != b.size()) throw new std::string("почленна€ операци€ над векторами разного размера недопустима");
	MatlabVector result = MatlabVector(a.size());
	for (uint i = 0; i < a.size(); i++) result[i] = a[i] * b[i];
	return result;
}

MatlabVector operator/(MatlabVector a, MatlabVector b)
{
	if (a.size() != b.size()) throw new std::string("почленна€ операци€ над векторами разного размера недопустима");
	MatlabVector result = MatlabVector(a.size());
	for (uint i = 0; i < a.size(); i++) result[i] = a[i] / b[i];
	return result;
}

MatlabVector MatlabVector::operator+(num a)
{
	MatlabVector result = MatlabVector(this->size());
	for (uint i = 0; i < this->size(); i++) result[i] = (*this)[i] + a;
	return result;
}

MatlabVector MatlabVector::operator*(num a)
{
	MatlabVector result = MatlabVector(this->size());
	for (uint i = 0; i < this->size(); i++) result[i] = (*this)[i] * a;
	return result;
}

MatlabVector::operator Json::Value() const
{
	Json::Value res = Json::arrayValue;
	for (auto i = this->begin(); i != this->end(); i++)
	{
		res.append(*i);
	}
	return res;
}

MatlabVector MatlabVector::getModule()
{
	MatlabVector res = MatlabVector(this->size());
	for (int i = 0; i < this->size(); i++)
	{
		res[i] = (*this)[i] * abs((*this)[i]);
	}
	return res;
}
