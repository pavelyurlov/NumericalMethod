#include "InputParser.h"
#include <fstream>
#include "json\json.h"

using std::string;
using std::vector;

struct field
{
	string name_in_file;
	string name_in_is;

	field(string name_in_file, string name_in_is):
		name_in_file(name_in_file),
		name_in_is(name_in_is)
	{}
};

void readVer1(Json::Value &root, std::vector<field>::iterator now, std::vector<field>::iterator end, std::vector<InputSet> &result, InputSet current = InputSet())  //  string fieldName, T &field)
{
	if (root.isMember(now->name_in_file))
	{
		vector<num> vals;
		if (false) // TODO: если там перебор
		{}
		else
		{
			vals.push_back(root[now->name_in_file].asFloat());
		}
		std::vector<field>::iterator next = now;
		next++;
		for (num val : vals)
		{
			current.insert(now->name_in_is, val);
			if (next == end)
			{
				result.push_back(current);
			}
			else
			{
				readVer1(root, next, end, result, current);
			}
		}
	}
	else
	{
		// TODO: throw Exception;
	}

	// TODO: Добавить обработку пунктов 8 и 9.

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

	std::vector<field> inputFields;
	inputFields.push_back(field("SizeOfArea", "A"));
	inputFields.push_back(field("NumberOfGridDots", "N"));
	inputFields.push_back(field("ClosureConstantAlpha", "max_iter"));
	inputFields.push_back(field("DeathRate1", "a"));
	inputFields.push_back(field("DeathRate2", "sw11"));
	inputFields.push_back(field("BirthRate1", "sw12"));
	inputFields.push_back(field("BirthRate2", "sw21"));
	inputFields.push_back(field("DeathRate1To1", "sw22"));
	inputFields.push_back(field("DeathRate1To2", "sm1"));
	inputFields.push_back(field("DeathRate2To1", "sm2"));
	inputFields.push_back(field("DeathRate2To2", "b1"));
	inputFields.push_back(field("SigmaBirth1", "b2"));
	inputFields.push_back(field("SigmaBirth2", "d1"));
	inputFields.push_back(field("SigmaDeath1To1", "d2"));
	inputFields.push_back(field("SigmaDeath1To2", "d11"));
	inputFields.push_back(field("SigmaDeath2To1", "d12"));
	inputFields.push_back(field("SigmaDeath2To2", "d22"));


	// Читаем поля
	std::vector<InputSet> result;
	readVer1(root, inputFields.begin(), inputFields.end(), result);
	return result;

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
	//default:
		// TODO: Бросить исключение о неизвестном формате
	}

	return result;
}
