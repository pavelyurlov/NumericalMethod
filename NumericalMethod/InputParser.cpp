#include "InputParser.h"
#include <fstream>
#include "json\json.h"

template <typename T>
void readFieldVersion1(Json::Value root, std::string fieldName, T &field)
{
	// TODO: Добавить обработку пунктов 8 и 9.

	if (root.isMember(fieldName))
	{
		// А это вообще поддерживается языком?
		switch (T)
		{
		case int:
			field = root[fieldName].asInt();
			break;
		case float:
			field = root[fieldName].asFloat();
			break;
		default:
			// TODO: Кинуть исключение
		}
	}
	else
	{
		// TODO: Кинуть исключение
	}
}

std::vector<InputSet> parseVersion1(Json::Value root)
{
	// Для лучшего понимания смотри "Формат входного файла численного метода, версия 1"

	// Проверяем, является ли файл ссылкой
	if (root.isMember("InputFileName"))
	{
		// TODO: Проверить наличие других полей, если есть, кинуть исключение

		std::string inputFileName = root["InputFileName"].asString();
		return InputParser::Parse(inputFileName);
	}

	// Читаем поля
	InputSet is;
	readFieldVersion1(root, "SizeOfArea", is.A);
	readFieldVersion1(root, "NumberOfGridDots", is.N);
	readFieldVersion1(root, "ClosureConstantAlpha", is.a);
	readFieldVersion1(root, "DeathRate1", is.d1);
	readFieldVersion1(root, "DeathRate2", is.d2);
	readFieldVersion1(root, "BirthRate1", is.b1);
	readFieldVersion1(root, "BirthRate2", is.b2);
	readFieldVersion1(root, "DeathRate1To1", is.d11);
	readFieldVersion1(root, "DeathRate1To2", is.d12);
	readFieldVersion1(root, "DeathRate2To1", is.d21);
	readFieldVersion1(root, "DeathRate2To2", is.d22);
	readFieldVersion1(root, "SigmaBirth1", is.sm1);
	readFieldVersion1(root, "SigmaBirth2", is.sm2);
	readFieldVersion1(root, "SigmaDeath1To1", is.sw11);
	readFieldVersion1(root, "SigmaDeath1To2", is.sw12);
	readFieldVersion1(root, "SigmaDeath2To1", is.sw21);
	readFieldVersion1(root, "SigmaDeath2To2", is.sw22);

	// TODO: Добавить чтение полей с настройками
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

	std::vector<InputSet> result;

	// Читаем файл
	switch (format)
	{
	case 1:
		result = parseVersion1(root);
		break;
	//sdefault:
		// TODO: Бросить исключение о неизвестном формате
	}

	return result;



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
