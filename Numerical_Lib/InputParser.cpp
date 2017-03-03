#include "InputParser.h"
#include <fstream>
#include "json\json.h"

using std::string;
using std::vector;

struct field
{
	string name_in_file;
	string name_in_is;

	field(string name_in_file, string name_in_is) :
		name_in_file(name_in_file),
		name_in_is(name_in_is)
	{}
};

// рекурсивно читаем файл, создаём все инпутсеты и кладём их в аргумент result
// аргументы:
// 1. root - исходный файл. По ссылке для оптимизации.
// 2. now - итератор на текущий аргумент.
// 3. end - итератор на последний аргумент.
// 4. result - результат. По ссылке, така как возвращается. Функция добавляет всё в конец, ничего не стирая и не создавая.
// 5. current - внутреннее. Текущий инпутсет.
void readVer1(Json::Value &root, std::vector<field>::iterator now, std::vector<field>::iterator end, std::vector<InputSet> &result, InputSet current = InputSet())
{
	// если поле не найдётся - это очень плохо, это ошибка
	if (root.isMember(now->name_in_file))
	{
		vector<num> vals;
		if (root[now->name_in_file].isMember("begin")) // если это перебор
		{
			// TODO: Добавить обработку пункта 9. Сложно, но возможно.

			// если что-то забыто, это ошибка
			if (!(root[now->name_in_file].isMember("begin") &&
				root[now->name_in_file].isMember("end") &&
				root[now->name_in_file].isMember("step")))
				throw std::exception("[InputParser -- readVer1] В файле нет ожидаемого begin, end или step");

			num begin = root[now->name_in_file]["begin"].asFloat();
			num end = root[now->name_in_file]["end"].asFloat();
			num step = root[now->name_in_file]["step"].asFloat();

			for (num i = begin; i < end; i += step)
			{
				vals.push_back(i);
			}
		}
		else // если это тупо одно значение
		{
			vals.push_back(root[now->name_in_file].asFloat());
		}

		// берём следующий шаг по массиву аргументов, либо записываем результат, если это последний
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
		throw std::exception("[InputParser -- readVer1] В файле нет ожидаемого имени");
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

	// TODO: Всё, что относится к InputSet, надо вынести в константы
	std::vector<field> inputFields;
	inputFields.push_back(field("SizeOfArea", "A"));
	inputFields.push_back(field("NumberOfGridDots", "N"));
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

	if (!(root.isMember("Dimentions") &&
		root.isMember("PrintD") &&
		root.isMember("MaxIterations") &&
		root.isMember("NumberOfKinds")))
		throw std::exception("[InputParser -- parseVersion1] Нету полей настроек: Dimentions, PrintD, MaxIterations, или NumberOfKinds");
	Preferences::dimentions = root["Dimentions"].asInt();
	Preferences::print_D = root["PrintD"].asBool();
	Preferences::max_iter = root["MaxIterations"].asInt();
	switch (root["NumberOfKinds"].asInt())
	{
	case 1: Preferences::one_kind = true;	break;
	case 2:	Preferences::one_kind = false;	break;
	default: throw std::exception("[InputParser -- parseVersion1] Количество видов должно быть 1 или 2");
	}
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
