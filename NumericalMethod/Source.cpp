#include "..\Numerical_Lib\InputParser.h"

#include <fstream>
#include <iostream>

void main()
{
	std::ifstream file = std::ifstream("..\\Num_UnitTest\\TestFiles\\1.json");
	std::cout << file.good();
	std::vector<InputSet> res = InputParser::Parse(".\TestFiles\1.json");
}