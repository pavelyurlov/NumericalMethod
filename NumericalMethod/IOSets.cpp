#include "IOSets.h"

std::ostream & operator<<(std::ostream &o, OutputSet &s)
{
	o << "rh:" << s.rh[50] << " D11:" << s.D11[50] << " D12:" << s.D12[50] 
		<< " D22:" << s.D22[50] << " y11:" << s.y11 << " y12:" << s.y12 
		<< " y21:" << s.y21 << " y22:" << s.y22 << " N1:" << s.N1 << " N2:"<< s.N2 << std::endl;
	return o;
}
