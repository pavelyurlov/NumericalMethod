#include "..\Numerical_Lib\InputParser.h"

#include <fstream>
#include <iostream>

void main()
{
	//std::ifstream file = std::ifstream("..\\Num_UnitTest\\TestFiles\\test_normal_file.json");
	//std::cout << file.good();
	//std::vector<InputSet> res = InputParser::Parse("..\\Num_UnitTest\\TestFiles\\test_normal_file.json");

	InputSet is;
	is.insert("N", 5);
}