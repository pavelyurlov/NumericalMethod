#include "RadialDistribution.h"

#define _USE_MATH_DEFINES

#include <numeric>
#include <math.h>



RadialDistribution::RadialDistribution(uint dimentions, uint numOfPoints, num distBetwZeroAndEdge)
{
	m_dim = dimentions;
	m_dist = distBetwZeroAndEdge;
	m_data = std::vector<num>(numOfPoints);
}

RadialDistribution::~RadialDistribution()
{
}

RadialDistribution operator+(RadialDistribution a, RadialDistribution b) { operationPerMember(a, b, RadialDistribution::PLUS ); }
RadialDistribution operator-(RadialDistribution a, RadialDistribution b) { operationPerMember(a, b, RadialDistribution::MINUS); }
RadialDistribution operator*(RadialDistribution a, RadialDistribution b) { operationPerMember(a, b, RadialDistribution::MULT ); }
RadialDistribution operator/(RadialDistribution a, RadialDistribution b) { operationPerMember(a, b, RadialDistribution::DIV  ); }

RadialDistribution RadialDistribution::operator+(num a) { operationWithNumber(a, RadialDistribution::PLUS); }
RadialDistribution RadialDistribution::operator*(num a) { operationWithNumber(a, RadialDistribution::MULT); }

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
		for (uint i = 0; i < getNumOfPoints(); i++) { res += 2 * M_PI * getCoord(i) * m_data[i] * getStepBetweenPoints(); }
		break;
	case 3:
		for (uint i = 0; i < getNumOfPoints(); i++) { res += 4 * M_PI * getCoord(i) * getCoord(i) * m_data[i] * getStepBetweenPoints(); }
		break;
	default: throw new std::string("[RadialDistribution.cpp -- CountIntegral] неизвестна€ размерность"); break;
	}
	return res;
}

num count_one_dim_int(std::vector<num> &vec, num step)
{
	return step * std::accumulate(vec.begin(), vec.end(), 0.0);
}

uint RadialDistribution::getNumOfPoints() { return m_data.size(); }

num RadialDistribution::getStepBetweenPoints() { return (m_dist * 2) / getNumOfPoints(); }

num RadialDistribution::getCoord(uint index) { return -m_dist + getStepBetweenPoints() * index; }

RadialDistribution operationPerMember(RadialDistribution a, RadialDistribution b, int type)
{
	if (a.getNumOfPoints() != b.getNumOfPoints()) throw new std::string("[RadialDistribution.cpp -- operationPerMember] почленна€ операци€ над распределени€ми разного размера недопустима");
	if (a.m_dim != b.m_dim) throw new std::string("[RadialDistribution.cpp -- operationPerMember] почленна€ операци€ над распределени€ми разных размерностей недопустима");
	RadialDistribution result = RadialDistribution(a.m_dim, a.getNumOfPoints());
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
	RadialDistribution result = RadialDistribution(m_dim, getNumOfPoints());
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
