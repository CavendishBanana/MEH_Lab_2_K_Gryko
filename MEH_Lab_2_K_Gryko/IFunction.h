#pragma once

#include <vector>

class IFunction
{
public:
	virtual double operator()(const std::vector<double>& vect) = 0;
	virtual ~IFunction();
};

