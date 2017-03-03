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

// ���������� ������ ����, ������ ��� ��������� � ����� �� � �������� result
// ���������:
// 1. root - �������� ����. �� ������ ��� �����������.
// 2. now - �������� �� ������� ��������.
// 3. end - �������� �� ��������� ��������.
// 4. result - ���������. �� ������, ���� ��� ������������. ������� ��������� �� � �����, ������ �� ������ � �� ��������.
// 5. current - ����������. ������� ��������.
void readVer1(Json::Value &root, std::vector<field>::iterator now, std::vector<field>::iterator end, std::vector<InputSet> &result, InputSet current = InputSet())
{
	// ���� ���� �� ������� - ��� ����� �����, ��� ������
	if (root.isMember(now->name_in_file))
	{
		vector<num> vals;
		if (root[now->name_in_file].isMember("begin")) // ���� ��� �������
		{
			// TODO: �������� ��������� ������ 9. ������, �� ��������.

			// ���� ���-�� ������, ��� ������
			if (!(root[now->name_in_file].isMember("begin") &&
				root[now->name_in_file].isMember("end") &&
				root[now->name_in_file].isMember("step")))
				throw std::exception("[InputParser -- readVer1] � ����� ��� ���������� begin, end ��� step");

			num begin = root[now->name_in_file]["begin"].asFloat();
			num end = root[now->name_in_file]["end"].asFloat();
			num step = root[now->name_in_file]["step"].asFloat();

			for (num i = begin; i < end; i += step)
			{
				vals.push_back(i);
			}
		}
		else // ���� ��� ���� ���� ��������
		{
			vals.push_back(root[now->name_in_file].asFloat());
		}

		// ���� ��������� ��� �� ������� ����������, ���� ���������� ���������, ���� ��� ���������
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
		throw std::exception("[InputParser -- readVer1] � ����� ��� ���������� �����");
	}
}

std::vector<InputSet> parseVersion1(Json::Value root)
{
	// ��� ������� ��������� ������ "������ �������� ����� ���������� ������, ������ 1"

	// ���������, �������� �� ���� �������
	if (root.isMember("InputFileName"))
	{
		// TODO: ��������� ������� ������ �����, ���� ����, ������ ����������

		std::string inputFileName = root["InputFileName"].asString();
		return InputParser::Parse(inputFileName);
	}

	// TODO: ��, ��� ��������� � InputSet, ���� ������� � ���������
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


	// ������ ����
	std::vector<InputSet> result;
	readVer1(root, inputFields.begin(), inputFields.end(), result);
	return result;

	if (!(root.isMember("Dimentions") &&
		root.isMember("PrintD") &&
		root.isMember("MaxIterations") &&
		root.isMember("NumberOfKinds")))
		throw std::exception("[InputParser -- parseVersion1] ���� ����� ��������: Dimentions, PrintD, MaxIterations, ��� NumberOfKinds");
	Preferences::dimentions = root["Dimentions"].asInt();
	Preferences::print_D = root["PrintD"].asBool();
	Preferences::max_iter = root["MaxIterations"].asInt();
	switch (root["NumberOfKinds"].asInt())
	{
	case 1: Preferences::one_kind = true;	break;
	case 2:	Preferences::one_kind = false;	break;
	default: throw std::exception("[InputParser -- parseVersion1] ���������� ����� ������ ���� 1 ��� 2");
	}
}

std::vector<InputSet> InputParser::Parse(std::string filename)
{
	// ��� ������� ��������� ������ "������ �������� ����� ���������� ������"

	// ��������� ����
	// TODO: ���, ���� ���� �� ������?
	std::ifstream file = std::ifstream(filename);

	// ������ ������ �����
	// TODO: ���, ���� ������ ���?
	Json::Value root;
	file >> root;
	int format = root["InputFileFormat"].asInt();

	std::vector<InputSet> result;

	// ������ ����
	switch (format)
	{
	case 1:
		result = parseVersion1(root);
		break;
		//default:
			// TODO: ������� ���������� � ����������� �������
	}

	return result;
}
