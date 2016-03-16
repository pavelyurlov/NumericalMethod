#pragma once

#include "minitypes.h"
#include "MatlabVector.h"
#include "./json/json.h"
#include <iostream>

struct InputSet
{
	num A;
	uint N;
	uint max_iter;
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
		root["max_iter"] = this->max_iter;
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
};

struct OutputSet
{
	MatlabVector rh, D11, D12, D22;
	num y11, y12, y21, y22, N1, N2;
	operator Json::Value() const
	{
		Json::Value root;
		root["rh"] = this->rh;
		root["D11"] = this->D11;
		root["D12"] = this->D12;
		root["D22"] = this->D22;
		root["y11"] = this->y11;
		root["y12"] = this->y12;
		root["y21"] = this->y21;
		root["y22"] = this->y22;
		root["N1"] = this->N1;
		root["N2"] = this->N2;
		return root;
	}
};

std::ostream& operator<<(std::ostream&, OutputSet&);
std::ostream& operator<<(std::ostream&, InputSet&);
