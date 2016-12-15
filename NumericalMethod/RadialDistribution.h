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

	RadialDistribution(uint dimentions, uint numOfPoints);
	~RadialDistribution();

	RadialDistribution friend operator+(RadialDistribution a, RadialDistribution b);
	RadialDistribution friend operator-(RadialDistribution a, RadialDistribution b);
	RadialDistribution friend operator*(RadialDistribution a, RadialDistribution b);
	RadialDistribution friend operator/(RadialDistribution a, RadialDistribution b);
	RadialDistribution operator+(num a);
	RadialDistribution operator*(num a);
	operator Json::Value() const;
	num CountIntegral();
private:
	std::vector<num> m_data;
	uint m_dim;

	uint getNumOfPoints();
	num getStepBetweenPoints();
	num getCoord(uint index);
	RadialDistribution friend operationPerMember(RadialDistribution a, RadialDistribution b, int type);
	RadialDistribution operationWithNumber(num a, int type);
};

