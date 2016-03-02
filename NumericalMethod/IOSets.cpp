#include "IOSets.h"
#include "./json/json.h"


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
