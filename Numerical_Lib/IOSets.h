#pragma once

#include "minitypes.h"
#include "MatlabVector.h"
#include "Distribution1D.h"
#include "Preferences.h"
#include "Partition.h"

#include "./json/json.h"
#include <iostream>

/*!
@brief Входные данные численного метода
@details Структура, в которой хранятся параметры для одного запуска численного метода
*/
struct InputSet
{
	num A;
	uint N;
	//uint max_iter;
	num a;
	num sw11;
	num sw12;
	num sw21;
	num sw22;
	num sm1;
	num sm2;
	num b1;
	num b2;
	num d1;
	num d2;
	num d11;
	num d12;
	num d21;
	num d22;
	operator Json::Value() const
	{
		Json::Value root;
		root["A"] = this->A;
		root["N"] = this->N;
		//root["max_iter"] = this->max_iter;
		root["a"] = this->a;
		root["sw11"] = this->sw11;
		root["sw12"] = this->sw12;
		root["sw21"] = this->sw21;
		root["sw22"] = this->sw22;
		root["sm1"] = this->sm1;
		root["sm2"] = this->sm2;
		root["b1"] = this->b1;
		root["b2"] = this->b2;
		root["d1"] = this->d1;
		root["d2"] = this->d2;
		root["d11"] = this->d11;
		root["d12"] = this->d12;
		root["d22"] = this->d22;
		return root;
	}
	template <typename T>
	void insert(std::string field, T value);
};

/*!
@brief Выходные данные численного метода
@details Структура, в которой хранятся результаты одного запуска численного метода
*/
struct OutputSet
{
	MatlabVector rh;
	Distribution1D D11, D12, D22;
	num y11, y12, y21, y22, N1, N2;
	OutputSet(Partition &p) : D11(p), D12(p), D22(p) {}
	operator Json::Value() const
	{
		Json::Value root;
		if (Preferences::print_D)
		{
			root["rh"] = this->rh;
			root["D11"] = this->D11;
			if (!Preferences::one_kind)
			{
				root["D12"] = this->D12;
				root["D22"] = this->D22;
			}
		}
		root["y11"] = this->y11;
		if (!Preferences::one_kind)
		{
			root["y12"] = this->y12;
			root["y21"] = this->y21;
			root["y22"] = this->y22;
			root["N2"] = this->N2;
		}
		root["N1"] = this->N1;
		return root;
	}
};

std::ostream& operator<<(std::ostream&, OutputSet&);
std::ostream& operator<<(std::ostream&, InputSet&);

template<typename T>
inline void InputSet::insert(const std::string field, T value)
{
	if (field == "A")		{ A = value; return;		}
	if (field == "N")		{ N = value; return;		}
	if (field == "a")		{ a = value; return;		}
	if (field == "sw11")	{ sw11 = value; return;		}
	if (field == "sw12")	{ sw12 = value; return;		}
	if (field == "sw21")	{ sw21 = value; return;		}
	if (field == "sw22")	{ sw22 = value; return;		}
	if (field == "sm1")		{ sm1 = value; return;		}
	if (field == "sm2")		{ sm2 = value; return;		}
	if (field == "b1")		{ b1 = value; return;		}
	if (field == "b2")		{ b2 = value; return;		}
	if (field == "d1")		{ d1 = value; return;		}
	if (field == "d2")		{ d2 = value; return;		}
	if (field == "d11")		{ d11 = value; return;		}
	if (field == "d12")		{ d12 = value; return;		}
	if (field == "d21")		{ d21 = value; return;		}
	if (field == "d22")		{ d22 = value; return;		}
	throw std::exception("[IOSets -- InputSet::insert] ѕопытка вставки пол¤ с неправильным названием");
}
