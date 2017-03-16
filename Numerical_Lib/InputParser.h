#pragma once

#include "IOSets.h"


const std::wstring ERROR_INPUT_PARSER_FILE_DOES_NOT_EXIST		= L"[InputParser] ���� �� ����������.";
const std::wstring ERROR_INPUT_PARSER_JSON_CANT_READ_VERSION	= L"[InputParser] ���������� ��������� ������ �����.";
const std::wstring ERROR_INPUT_PARSER_UNKNOWN_VERSION			= L"[InputParser] ����������� ������ �����.";

class InputParser
{
public:
	_declspec(dllimport) static std::vector<InputSet> Parse(std::string filename);
};

