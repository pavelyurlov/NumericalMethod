#pragma once

#include "IOSets.h"



class InputParser
{
public:
	_declspec(dllimport) static std::vector<InputSet> Parse(std::string filename);
};

