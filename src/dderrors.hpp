#ifndef DDERRORS_HPP
#define DDERRORS_HPP

#include <exception>

using namespace std;

class ModelError: public exception
{
    virtual const char* what() const throw()
	{
	    return "Model couldn't deal your input.";
	}
};

class InputError: public exception
{
    virtual const char* what() const throw()
	{
	    return "We couldn't understand your input.";
	}
public:
    const char* name() const
	{
	    return "InputError";
	}
};

#endif
