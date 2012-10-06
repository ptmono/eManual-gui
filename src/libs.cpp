#include "libs.hpp"

#include "boost/algorithm/string/predicate.hpp"

bool stringToBool(const string & s)
{
    if (boost::iequals(s.c_str(), "true"))
	return true;
    else
	return false;
}
