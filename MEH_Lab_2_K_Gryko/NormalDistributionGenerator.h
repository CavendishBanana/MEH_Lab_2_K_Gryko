#pragma once
#include "INumberGenerator.h"
#include <random>
class NormalDistributionGenerator :
    public INumberGenerator
{
private:
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;

public:
    NormalDistributionGenerator(double mi, double sigma);
    virtual ~NormalDistributionGenerator();
    double generateNumber() override;

};

