#include "RadialDistribution.h"

#define _USE_MATH_DEFINES

#include <numeric>
#include <math.h>


num count_one_dim_int(std::vector<num> &vec, num step);


RadialDistribution::RadialDistribution(uint dimentions, uint numOfPoints, num distBetwZeroAndEdge, num(*initFunc)(num))
{
	m_dim = dimentions;
	m_dist = distBetwZeroAndEdge;
	m_data = std::vector<num>(numOfPoints);
	for (uint i = 0; i < GetNumOfPoints(); i++) { m_data[i] = initFunc(getCoord(i)); }
}

RadialDistribution::~RadialDistribution()
{
}

RadialDistribution operator+(RadialDistribution a, RadialDistribution b) { return operationPerMember(a, b, RadialDistribution::PLUS ); }
RadialDistribution operator-(RadialDistribution a, RadialDistribution b) { return operationPerMember(a, b, RadialDistribution::MINUS); }
RadialDistribution operator*(RadialDistribution a, RadialDistribution b) { return operationPerMember(a, b, RadialDistribution::MULT ); }
RadialDistribution operator/(RadialDistribution a, RadialDistribution b) { return operationPerMember(a, b, RadialDistribution::DIV  ); }

RadialDistribution RadialDistribution::operator+(num a) { return operationWithNumber(a, RadialDistribution::PLUS); }
RadialDistribution RadialDistribution::operator*(num a) { return operationWithNumber(a, RadialDistribution::MULT); }

RadialDistribution operator+(num a, RadialDistribution b) { return b + a; }
RadialDistribution operator*(num a, RadialDistribution b) { return b * a; }

num RadialDistribution::CountIntegral()
{
	num h = getStepBetweenPoints();
	num res = 0;
	switch (m_dim)
	{
	case 1:
		res = count_one_dim_int(m_data, getStepBetweenPoints());
		break;
	case 2:
		for (uint i = 0; i < GetNumOfPoints(); i++) { res += 2 * M_PI * getCoord(i) * m_data[i] * getStepBetweenPoints(); }
		break;
	case 3:
		for (uint i = 0; i < GetNumOfPoints(); i++) { res += 4 * M_PI * getCoord(i) * getCoord(i) * m_data[i] * getStepBetweenPoints(); }
		break;
	default: throw new std::string("[RadialDistribution.cpp -- CountIntegral] неизвестна€ размерность"); break;
	}
	return res;
}

num count_one_dim_int(std::vector<num> &vec, num step)
{
	return step * std::accumulate(vec.begin(), vec.end(), 0.0);
}

uint RadialDistribution::GetNumOfPoints() { return m_data.size(); }

RadialDistribution RadialDistribution::GetRadiusDistribution()
{
	return RadialDistribution(m_dim, GetNumOfPoints(), m_dist, [](num r) -> num { return r; });
}

num RadialDistribution::getStepBetweenPoints() { return (m_dist * 2) / GetNumOfPoints(); }

num RadialDistribution::getCoord(uint index) { return -m_dist + getStepBetweenPoints() * index; }

RadialDistribution operationPerMember(RadialDistribution a, RadialDistribution b, int type)
{
	if (a.GetNumOfPoints() != b.GetNumOfPoints()) throw new std::string("[RadialDistribution.cpp -- operationPerMember] почленна€ операци€ над распределени€ми разного размера недопустима");
	if (a.m_dim != b.m_dim) throw new std::string("[RadialDistribution.cpp -- operationPerMember] почленна€ операци€ над распределени€ми разных размерностей недопустима");
	RadialDistribution result = RadialDistribution(a.m_dim, a.GetNumOfPoints(), a.m_dist);
	for (uint i = 0; i < a.m_data.size(); i++)
	{
		switch (type)
		{
		case RadialDistribution::PLUS:  result.m_data[i] = a.m_data[i] + b.m_data[i]; break;
		case RadialDistribution::MINUS: result.m_data[i] = a.m_data[i] + b.m_data[i]; break;
		case RadialDistribution::MULT:  result.m_data[i] = a.m_data[i] + b.m_data[i]; break;
		case RadialDistribution::DIV:   result.m_data[i] = a.m_data[i] + b.m_data[i]; break;
		default: throw new std::string("[RadialDistribution.cpp -- operationPerMember] неизвестный тип операции"); break;
		}
	}
	return result;
}

RadialDistribution RadialDistribution::operationWithNumber(num a, int type)
{
	RadialDistribution result = RadialDistribution(m_dim, GetNumOfPoints(), m_dist);
	for (uint i = 0; i < m_data.size(); i++)
	{
		switch (type)
		{
		case RadialDistribution::PLUS: result.m_data[i] = m_data[i] + a; break;
		case RadialDistribution::MULT: result.m_data[i] = m_data[i] + a; break;
		default: throw new std::string("[RadialDistribution.cpp -- operationWithNumber] неизвестный тип операции"); break;
		}
	}
	return result;
}

num RadialDistribution::Zero(num r) { return 0; }

RadialDistribution::operator Json::Value() const
{
	Json::Value res = Json::arrayValue;
	for (auto i = m_data.begin(); i != m_data.end(); i++)
	{
		res.append(*i);
	}
	return res;
}