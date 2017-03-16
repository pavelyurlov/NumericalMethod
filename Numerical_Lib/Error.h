#pragma once

#include <string>

class Error
{
public:
	_declspec(dllimport) Error(std::wstring _message);
	_declspec(dllimport) std::wstring GetMessage();
private:
	std::wstring m_message;
};

