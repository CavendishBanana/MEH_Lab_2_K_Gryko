#include "AckleyFunction.h"

double AckleyFunction::operator()(const std::vector<double>& x)
{
    double oneByDimensionality = 1.0 / static_cast<double>(x.size());
    double sum1 = 0.0;
    double sum2 = 0.0;
    auto actionSum = [&sum1, &sum2](const double& x) { sum1+= (x * x); sum2 += cos(twoPi * x); };
    std::for_each(x.begin(), x.end(), actionSum);
    return ePlus20 - 20.0 * exp(-0.2 * sqrt(oneByDimensionality) * sum1) - exp(oneByDimensionality * sum2);
}
// exp(1)+20-20*exp(-0.2*sqrt((1/n)*sum(x(i)^2)))-exp((1/n)*sum(cos(2*Pi*x(i))))
AckleyFunction::~AckleyFunction()
{
}
