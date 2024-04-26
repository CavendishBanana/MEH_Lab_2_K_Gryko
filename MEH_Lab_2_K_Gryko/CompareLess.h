#pragma once

#include "ICompare.h"
class CompareLess :public ICompare
{
public:
	bool leftIsBetterThanRight(double left, double right);
	~CompareLess();
};

