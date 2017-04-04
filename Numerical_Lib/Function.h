#pragma once

#include "minitypes.h"

/*!
@brief Интерфейсный класс для использования математических функций
@details В классе-реализации должен присутствовать метод At, который позволяет получить значение функции в точке
*/
class Function
{
public:
	/*!
	Возвращает значение функции в точке
	@param[in] x Координата, в которой необходимо вычислить значение функции
	@return Значение функции в координате x
	*/
	virtual num At(num) const = 0;
};

/*!
@brief Нормальное распределение
@details @snippet Function.cpp NormalFunction1D
@todo перевести формулу в документации в LateX
*/
class NormalFunction1D : public Function
{
public:
	_declspec(dllimport) NormalFunction1D(num param, num sigma);
	virtual num At(num) const override;
private:
	const num m_param;
	const num m_sigma;
};