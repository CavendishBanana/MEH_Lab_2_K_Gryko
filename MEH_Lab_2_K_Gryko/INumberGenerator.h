#pragma once
class INumberGenerator
{
public:
	virtual double generateNumber() = 0;
	virtual ~INumberGenerator();
};

