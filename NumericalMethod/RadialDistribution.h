#pragma once
#include <vector>

#include "minitypes.h"
#include "json\json.h"

class RadialDistribution
{
public:
	static const int PLUS	= 0;
	static const int MINUS	= 1;
	static const int MULT	= 2;
	static const int DIV	= 3;
	static num Zero(num r);

	RadialDistribution(uint dimentions, uint numOfPoints, num distBetwZeroAndEdge, num (*initFunc)(num) = Zero);
	~RadialDistribution();

	RadialDistribution friend operator+(RadialDistribution a, RadialDistribution b);
	RadialDistribution friend operator-(RadialDistribution a, RadialDistribution b);
	RadialDistribution friend operator*(RadialDistribution a, RadialDistribution b);
	RadialDistribution friend operator/(RadialDistribution a, RadialDistribution b);
	RadialDistribution operator+(num a);
	RadialDistribution operator*(num a);
	RadialDistribution friend operator+(num a, RadialDistribution b);
	RadialDistribution friend operator*(num a, RadialDistribution b);
	operator Json::Value() const;
	num CountIntegral();
private:
	std::vector<num> m_data;
	uint m_dim;
	num m_dist; // размер одной половины

	uint getNumOfPoints();
	num getStepBetweenPoints();
	num getCoord(uint index);
	RadialDistribution friend operationPerMember(RadialDistribution a, RadialDistribution b, int type);
	RadialDistribution operationWithNumber(num a, int type);
};

