#pragma once

#include <vector>
#include "./json/json.h"
#include "minitypes.h"

/*!
@brief Старый класс, который надо удалить
@details Раньше использовался для хранения распределений, но не содержал контекста. Просто вектор с некоторыми переопределёнными операциями вроде поэлементного сложения. Сейчас заменён связкой Distribution - Partition
*/
class MatlabVector :
	public std::vector<num>
{
public:
	MatlabVector() {}
	MatlabVector(uint N) :std::vector<num>(N) {} // garbage inside?
	template <class InputIterator> MatlabVector(InputIterator first, InputIterator last) : vector<num>(first, last) {}
	friend MatlabVector operator+(MatlabVector a, MatlabVector b);
	friend MatlabVector operator-(MatlabVector a, MatlabVector b);
	friend MatlabVector operator*(MatlabVector a, MatlabVector b);
	friend MatlabVector operator/(MatlabVector a, MatlabVector b);
	MatlabVector operator+(num a);
	MatlabVector operator*(num a);
	operator Json::Value() const;
private:
	MatlabVector per_member_op(MatlabVector&, num(*op)(num, num));
	MatlabVector per_member_op(num, num(*op)(num, num));
};

MatlabVector operator*(num n, MatlabVector mv);

// 14.23 с лямбда-функциями
// 13.05 без лямбда-функций