#include <algorithm>
#include <cmath>

#include "Distribution1D.h"
#include "Error.h"

const num Distribution1D::COMPARE_EPSILON = 0.0001;

Distribution1D::Distribution1D(Partition &p, num n) :
	m_part(p)
{
	m_data = std::vector<num>(p.GetNum(), n);
}

Distribution1D::Distribution1D(Partition &p, Function &f) :
	m_part(p)
{
	m_data = std::vector<num>(p.GetNum());
	for (uint i = 0; i < m_data.size(); i++)
	{
		m_data[i] = f.At(m_part.ItoX(i));
	}
}

num Distribution1D::CountIntegral()
{
	num res = 0;
	for (num elem : m_data)
	{
		res += elem;
	}
	return res / m_part.GetStep();
}

num Distribution1D::MaxModule()
{
	num res = 0;
	for (num data : m_data)
	{
		res = std::max(res, fabs(data));
	}
	return res;
}

Distribution1D Distribution1D::operator2(Distribution1D &o, num(&f)(num, num))
{
	if (m_data.size() != o.m_data.size()) throw Error(ERROR_UNDESCRIBED);
	Distribution1D res = Distribution1D(m_part);
	for (uint i = 0; i < m_data.size(); i++)
	{
		res.m_data[i] = f(m_data[i], o.m_data[i]);
	}
	return res;
}

Distribution1D Distribution1D::operator2(num o, num(&f)(num, num)) // o - right
{
	Distribution1D res = Distribution1D(m_part);
	for (uint i = 0; i < m_data.size(); i++)
	{
		res.m_data[i] = f(m_data[i], o);
	}
	return res;
}

Distribution1D Distribution1D::operator2(num(&f)(num, num), num o) // o - left
{
	Distribution1D res = Distribution1D(m_part);
	for (uint i = 0; i < m_data.size(); i++)
	{
		res.m_data[i] = f(o, m_data[i]);
	}
	return res;
}

num plus(num a, num b) { return a + b; }
num minu(num a, num b) { return a - b; }
num mult(num a, num b) { return a * b; }
num divi(num a, num b) { return a / b; }
num righ(num a, num b) { return b; }

Distribution1D Distribution1D::operator+(Distribution1D &o) { return operator2(o, plus); }
Distribution1D Distribution1D::operator-(Distribution1D &o) { return operator2(o, minu); }
Distribution1D Distribution1D::operator*(Distribution1D &o) { return operator2(o, mult); }
Distribution1D Distribution1D::operator/(Distribution1D &o) { return operator2(o, divi); }
Distribution1D Distribution1D::operator=(Distribution1D &o) { return operator2(o, righ); }

Distribution1D Distribution1D::operator+(num o) { return operator2(o, plus); }
Distribution1D Distribution1D::operator*(num o) { return operator2(o, mult); }

Distribution1D operator+(num n, Distribution1D d) { return d.operator2(plus, n); }
Distribution1D operator*(num n, Distribution1D d) {	return d.operator2(mult, n); }

bool Distribution1D::operator==(Distribution1D &other)
{
	return (*this - other).MaxModule() < Distribution1D::COMPARE_EPSILON;
}

Distribution1D::operator Json::Value() const
{
	Json::Value res = Json::arrayValue;
	for (num data : m_data)
	{
		res.append(data);
	}
	return res;
}
