#include "Error.h"



Error::Error(std::wstring _message):
	m_message(_message)
{
}


std::wstring Error::GetMessage()
{
	return m_message;
}
