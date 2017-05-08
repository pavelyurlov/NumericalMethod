#pragma once

#include "macros.h"
#include "minitypes.h"
#include "Partition.h"
#include "Function.h"
#include "./json/json.h"

/*!
@brief Одномерное числовое распределение
@details В данном классе хранится одномерное точечное числовое распределение.
Точки, в которых определены значения данного распределения, находятся во внутренней ссылке на класс Partition.
Для того, чтобы выполнять какие-либо действия с двумя Distribution, они должны иметь один и тот же Partition.
Для инициализации точек используется класс Function.
Для класса определена операция свёртки. Она находится в классах FourierFFTW, FourierMKL.
*/
class Distribution1D
{
public:
	/*!
	@brief Конструктор с константой
	@details Создаёт распределение на основе разбиения и устанавливает значения всех точек в заданную константу.
	@param[in] p Одномерное разбиение, служащее основой будущего распределения.
	@param[in] n Константа, в которую устанавливаются все значения точек.
	*/
	MSTESTPREFIX Distribution1D(Partition&, num n = 0);

	/*!
	@brief Конструктор с функцией
	@details Создаёт распределение на основе разбиения и назначает каждой точке значение функции в этой точке.
	@param[in] p Одномерное разбиение, служащее основой будущего распределения.
	@param[in] f Функция, которая используется для вычисления значений точек.
	*/
	MSTESTPREFIX Distribution1D(Partition&, Function&);

	/*!
	@brief Получение количества точек в распределении
	@result Количество точек в распределении
	*/
	uint GetSize() { return m_data.size(); }

	/*!
	@brief Получение внутренних данных
	@result Массив, в котором хранятся точки распределения
	*/
	num* GetData() { return m_data.data(); }

	/*!
	@brief Подсчёт интеграла по распределению
	@result Значение интегральной суммы по распределению
	*/
	num CountIntegral();

	Distribution1D operator+ (Distribution1D&); //!< Сложение двух одинакого разбитых распределений
	Distribution1D operator- (Distribution1D&);	//!< Вычитание двух одинакого разбитых распределений
	Distribution1D operator* (Distribution1D&);	//!< Умножение двух одинакого разбитых распределений
	Distribution1D operator/ (Distribution1D&);	//!< Деление двух одинакого разбитых распределений
	Distribution1D operator+ (num); //!< Прибавление числа к распределению
	Distribution1D operator* (num); //!< Умножение распределения на число
	friend Distribution1D operator+(num, Distribution1D); //!< Прибавление числа к распределению
	friend Distribution1D operator*(num, Distribution1D); //!< Умножение распределения на число
	Distribution1D operator=(Distribution1D&);
	MSTESTPREFIX bool operator==(Distribution1D&);
	operator Json::Value() const;
private:
	static const num COMPARE_EPSILON; //!< Число, используемое для сравнения двух чисел из Distribution1D

	Partition & m_part;
	std::vector<num> m_data;
	Distribution1D operator2(Distribution1D&, num(&)(num, num));
	Distribution1D operator2(num, num(&)(num, num));
	Distribution1D operator2(num(&f)(num, num), num);

	num MaxModule();
};