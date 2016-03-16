#include "IOSets.h"
#include "./json/json.h"


std::ostream & operator<<(std::ostream &o, OutputSet &s)
{
	o << static_cast<Json::Value>(s);
	return o;
}

std::ostream & operator<<(std::ostream &o, InputSet &s)
{
	o << static_cast<Json::Value>(s);
	return o;
}
