#pragma once

#include "macros.h"
#include "IOSets.h"


const std::wstring ERROR_INPUT_PARSER_FILE_DOES_NOT_EXIST		= L"[InputParser] Файл не существует.";
const std::wstring ERROR_INPUT_PARSER_JSON_CANT_READ_VERSION	= L"[InputParser] Невозможно прочитать версию файла.";
const std::wstring ERROR_INPUT_PARSER_UNKNOWN_VERSION			= L"[InputParser] Неизвестная версия файла.";

/*!
@brief Чтение ввода из файла
@details Статический класс, который занимается чтением ввода из файла и генерацией InputSet
*/
class InputParser
{
public:
	MSTESTPREFIX static std::vector<InputSet> Parse(std::string filename);
};

