#pragma once

#include "minitypes.h"

class Function
{
public:
};

class NormalFunction : public Function
{
public:
	NormalFunction(num param, num sigma);
private:
	const num m_param;
	const num m_sigma;
};