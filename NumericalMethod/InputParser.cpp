#include "InputParser.h"
#include <fstream>
#include "json\json.h"


std::vector<InputSet> parseVersion1(Json::Value root)
{
	///
}

std::vector<InputSet> InputParser::Parse(std::string filename)
{
	// Для лучшего понимания смотри "Формат входного файла численного метода"

	// Открываем файл
	// TODO: Что, если файл не найден?
	std::ifstream file = std::ifstream(filename);

	// Читаем версию файла
	// TODO: Что, если версии нет?
	Json::Value root;
	file >> root;
	int format = root["InputFileFormat"].asInt();

	// Читаем файл
	switch (format)
	{
	case 1:
		parseVersion1(root);
		break;
	default:
		// TODO: Бросить исключение о неизвестном формате
	}



	InputSet is;

	//---------- - Model Constants--------
	is.sm1 = 0.04, is.sm2 = 0.06;
	is.b1 = 0.4; is.b2 = 0.4;
	is.d11 = 0.001;  is.d22 = 0.001; is.d12 = 0.001; is.d21 = 0.001;
	is.sw11 = 0.04, is.sw22 = 0.04, is.sw12 = 0.04, is.sw21 = 0.04;
	is.d1 = 0.2; is.d2 = 0.2;


	//----------Grid settings----------
	is.N = 100;
	is.A = 1;

	//-------- - Numerical method Constants------ -
	is.max_iter = 500;

	//-------- - Closure Constants------ -
	is.a = 0.4;



	std::vector<InputSet> input;


	for (num d12 = 0; d12 < 0.001; d12 += 0.001 * 0.05)
	{
		num sm2 = (d12 / 0.001) * 0.16;
		is.d12 = d12;
		is.sm2 = sm2;
		input.push_back(is);
	}

	return std::vector<InputSet>();
}
