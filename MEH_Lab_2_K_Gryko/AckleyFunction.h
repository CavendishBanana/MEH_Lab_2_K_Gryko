#pragma once
#include "IFunction.h"
#include <cmath>
#include <math.h>
#include <vector>
#include <algorithm>
#define MY_M_E 2.71828182845904523536
#define MY_2_PI 3.14159265358979323846
class AckleyFunction :
    public IFunction
{
private:
    constexpr static double ePlus20 = MY_M_E + 20.0;
    constexpr static double twoPi = 2.0 * MY_2_PI;
public:
    double operator()(const std::vector<double>& x) override;
    virtual ~AckleyFunction();
};

