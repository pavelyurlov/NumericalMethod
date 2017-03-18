#pragma once

#include "minitypes.h"
#include "Partition.h"
#include "Function.h"
#include "json\json.h"

class Distribution1D
{
public:
	Distribution1D(Partition&, num n = 0);
	Distribution1D(Partition&, Function&);

	uint GetSize() { return m_data.size(); }
	num* GetData() { return m_data.data(); }

	num CountIntegral();

	Distribution1D operator+ (Distribution1D&);
	Distribution1D operator- (Distribution1D&);
	Distribution1D operator* (Distribution1D&);
	Distribution1D operator/ (Distribution1D&);
	Distribution1D operator+ (num);
	Distribution1D operator* (num);
	friend Distribution1D operator+(num, Distribution1D);
	friend Distribution1D operator*(num, Distribution1D);
	Distribution1D operator=(Distribution1D&);
	operator Json::Value() const;
private:
	Partition & m_part;
	std::vector<num> m_data;
	Distribution1D operator2(Distribution1D&, num(&)(num, num));
	Distribution1D operator2(num, num(&)(num, num));
	Distribution1D operator2(num(&f)(num, num), num);

};
