#pragma once

#include <string>

#include "macros.h"

const std::wstring ERROR_UNDESCRIBED = L"[InputParser] Текст ошибки не написан.";

/*!
@brief Передача ошибок
*/
class Error
{
public:
	MSTESTPREFIX Error(std::wstring _message);
	MSTESTPREFIX std::wstring GetMessage();
private:
	std::wstring m_message;
};

