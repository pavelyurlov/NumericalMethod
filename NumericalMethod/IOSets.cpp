#include "IOSets.h"
#include "json\json.h"

InputSet::operator Json::Value() const
{
	Json::Value root;
	root["A"] = this->A;
	root["N"] = this->N;
	root["max_iter"] = this->max_iter;
	root["a"] = this->a;
	//root["f_***"] = this->f_m1***.rasterize(); //TODO
	root["b1"] = this->b1;
	root["b2"] = this->b2;
	root["d1"] = this->d1;
	root["d2"] = this->d2;
	root["d11"] = this->d11;
	root["d12"] = this->d12;
	root["d22"] = this->d22;
	return root;
}

OutputSet::operator Json::Value() const
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

std::ostream & operator<<(std::ostream &o, OutputSet &s)
{
	o << static_cast<Json::Value>(s);
	return o;

	/*o << "rh:" << s.rh[50] << " D11:" << s.D11[50] << " D12:" << s.D12[50]
		<< " D22:" << s.D22[50] << " y11:" << s.y11 << " y12:" << s.y12 
		<< " y21:" << s.y21 << " y22:" << s.y22 << " N1:" << s.N1 << " N2:"<< s.N2;*/
}

std::ostream & operator<<(std::ostream &o, InputSet &s)
{
	o << static_cast<Json::Value>(s);
	return o;

	/*o << 
	"A:" A;
	uint N;
	uint max_iter;
	num a;
	num(*f_w11)(num);
	num(*f_w12)(num);
	num(*f_w21)(num);
	num(*f_w22)(num);
	num(*f_m1)(num);
	num(*f_m2)(num);
	num b1;
	num b2;
	num d1;
	num d2;
	num d11;
	num d12;
	num d21;
	num d22;
	return o;*/
}
