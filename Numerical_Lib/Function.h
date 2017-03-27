#pragma once

#include "minitypes.h"

class Function
{
public:
	virtual num At(num) const = 0;
};

class NormalFunction1D : public Function
{
public:
	_declspec(dllimport) NormalFunction1D(num param, num sigma);
	virtual num At(num) const override;
private:
	const num m_param;
	const num m_sigma;
};