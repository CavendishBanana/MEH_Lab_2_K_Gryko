#include "NormalDistributionGenerator.h"

NormalDistributionGenerator::NormalDistributionGenerator(double mi, double sigma) :INumberGenerator(), generator(), distribution(mi, sigma)
{
}

NormalDistributionGenerator::~NormalDistributionGenerator()
{
}

double NormalDistributionGenerator::generateNumber()
{
	return distribution(generator);
}
