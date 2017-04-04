#pragma once

#include <string>

const std::wstring ERROR_UNDESCRIBED = L"[InputParser] Текст ошибки не написан.";

/*!
@brief Передача ошибок
*/
class Error
{
public:
	_declspec(dllimport) Error(std::wstring _message);
	_declspec(dllimport) std::wstring GetMessage();
private:
	std::wstring m_message;
};

