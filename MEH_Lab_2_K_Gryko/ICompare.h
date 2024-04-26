#pragma once
class ICompare
{
public:
	virtual bool leftIsBetterThanRight(double left, double right) = 0;
	virtual ~ICompare();
};

