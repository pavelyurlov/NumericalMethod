#pragma once

#include "IOSets.h"



class InputParser
{
public:
	static std::vector<InputSet> Parse(std::string filename);
};

