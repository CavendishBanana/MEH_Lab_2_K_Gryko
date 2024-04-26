// MEH_Lab_2_K_Gryko.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
// Krzysztof Gryko

#include <iostream>
#include <random>
#include <vector>
#include "IFunction.h"
#include "AckleyFunction.h"
#include "ICompare.h"
#include "CompareLess.h"
#include <algorithm>
#include <numeric>

using namespace std;

vector<double> generateNewSolution(const vector<double>& baseSolution, double neighbourhoodRange, default_random_engine& generator, normal_distribution<double>& distribution)
{
    vector<double> newSol = baseSolution;
    for_each(newSol.begin(), newSol.end(), [&neighbourhoodRange, &generator, &distribution](double& x) { x += neighbourhoodRange * distribution(generator); });
    return newSol;
}

vector<double> simpleGreatestIncrease(int dimensionality, int iterations, int numberOfGeneratedSolutions, double neghbourhoodRange, IFunction& criterionFunction, ICompare& compareCriterion)
{
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);
    vector<double> currentPoint = vector<double>(dimensionality, 0.0);
    for (int i = 0; i < dimensionality; i++)
    {
        currentPoint[i] = distribution(generator);
    }
    double critFunVal = criterionFunction(currentPoint);
    vector<vector<double>> newSolutions = vector<vector<double>>( numberOfGeneratedSolutions, vector<double>(1, 0.0) );
    vector<double> newSolsCritFunVals = vector<double>(numberOfGeneratedSolutions, 0.0);
    for (int i = 0; i < iterations; i++)
    {
        
        for (int j = 0; j < numberOfGeneratedSolutions; j++)
        {
            vector<double> newSolution = currentPoint;
            for (int k = 0; k < dimensionality; k++)
            {
                newSolution[k] += distribution(generator) * neghbourhoodRange;
            }
            newSolsCritFunVals[j] = criterionFunction(newSolution);
            newSolutions[j] = move(newSolution);
        
        }
        double bestValInNewSols = critFunVal;
        int bestValIdx = -1;
        for (int j = 0; j < numberOfGeneratedSolutions; j++)
        {
            if (compareCriterion.leftIsBetterThanRight(newSolsCritFunVals[j], bestValInNewSols))
            {
                bestValIdx = j;
                bestValInNewSols = newSolsCritFunVals[j];
            }
        }
        if (bestValIdx != -1)
        {
            currentPoint = newSolutions[bestValIdx];
            critFunVal = newSolsCritFunVals[bestValIdx];
        }

    }
    return currentPoint;
    
}

vector<double> simpleGreatestIncrease(const vector<double>& startPoint, int iterations, int numberOfGeneratedSolutions, 
    double neghbourhoodRange, IFunction& criterionFunction, ICompare& compareCriterion)
{
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);
    vector<double> currentPoint = startPoint;

    double critFunVal = criterionFunction(currentPoint);
    vector<vector<double>> newSolutions = vector<vector<double>>(numberOfGeneratedSolutions, vector<double>(1, 0.0));
    vector<double> newSolsCritFunVals = vector<double>(numberOfGeneratedSolutions, 0.0);
    for (int i = 0; i < iterations; i++)
    {

        for (int j = 0; j < numberOfGeneratedSolutions; j++)
        {
            vector<double> newSolution = currentPoint;
            for (int k = 0; k < currentPoint.size(); k++)
            {
                newSolution[k] += distribution(generator) * neghbourhoodRange;
            }
            newSolsCritFunVals[j] = criterionFunction(newSolution);
            newSolutions[j] = move(newSolution);

        }
        double bestValInNewSols = critFunVal;
        int bestValIdx = std::min_element(newSolsCritFunVals.begin(), newSolsCritFunVals.end()) - newSolsCritFunVals.begin();
        for (int j = 0; j < numberOfGeneratedSolutions; j++)
        {
            if (compareCriterion.leftIsBetterThanRight(newSolsCritFunVals[j], bestValInNewSols))
            {
                bestValIdx = j;
                bestValInNewSols = newSolsCritFunVals[j];
            }
        }
        if (bestValIdx != -1)
        {
            currentPoint = newSolutions[bestValIdx];
            critFunVal = newSolsCritFunVals[bestValIdx];
        }

    }
    return currentPoint;

}

vector<double> multipleStartGreatestIncrease(int dimensionality, int iterations, int numberOfStarts, int numberOfGeneratedSolutions, double neghbourhoodRange, IFunction& criterionFunction, ICompare& compareCriterion)
{
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);
    vector<double> currentPoint = vector<double>(dimensionality, 0.0);
    vector<double> globalBestSol = vector<double>(dimensionality, 0.0);
    for_each(globalBestSol.begin(), globalBestSol.end(), [&distribution, &generator](double& x) { x = distribution(generator); });
    double globalBestCritFunVal = criterionFunction(globalBestSol);
    double critFunVal = 0.0;
    vector<vector<double>> newSolutions = vector<vector<double>>(numberOfGeneratedSolutions, vector<double>(1, 0.0));
    vector<double> newSolsCritFunVals = vector<double>(numberOfGeneratedSolutions, 0.0);
    for (int i = 0; i < numberOfStarts; i++)
    {
        bool local = false;
        for (int i = 0; i < dimensionality; i++)
        {
            currentPoint[i] = distribution(generator);
        }
        critFunVal = criterionFunction(currentPoint);
        int bestSolutionIdx = -1;
        do
        {
            for_each(newSolutions.begin(), newSolutions.end(), [&currentPoint, neghbourhoodRange, &generator, &distribution](vector<double>& sol) {sol =
                generateNewSolution(currentPoint, neghbourhoodRange, generator, distribution); });
            for (int j = 0; j < numberOfGeneratedSolutions; j++)
            {
                newSolsCritFunVals[i] = criterionFunction(newSolutions[j]);
                if (compareCriterion.leftIsBetterThanRight(newSolsCritFunVals[j], critFunVal))
                {
                    bestSolutionIdx = j;
                    critFunVal = newSolsCritFunVals[j];
                }
            }
            if (bestSolutionIdx != -1 && compareCriterion.leftIsBetterThanRight(newSolsCritFunVals[bestSolutionIdx], globalBestCritFunVal))
            {
                globalBestSol = newSolutions[bestSolutionIdx];
                globalBestCritFunVal = newSolsCritFunVals[bestSolutionIdx];
            }
            else
            {
                local = true;
            }
        } while (!local);
        return globalBestSol;
    }


}

int main()
{   
    AckleyFunction ackleyFun = AckleyFunction();
    CompareLess compareCondition = CompareLess();
    
    int iterations = 1000, generatedSolutionsCount=20;
    double neighbourhoodRange = 0.2;

    vector<int> problemDimensionalities = vector<int>(2,0);
    problemDimensionalities[0] = 5;
    problemDimensionalities[1] = 8;
    int runsCount = 10;
    
    vector<double> criterionFunctionVals = vector<double>(runsCount, 0.0);

    cout << "Test simple max increase algorithm" << endl;
    for (int i = 0; i < problemDimensionalities.size(); i++)
    {
        for (int j = 0; j < runsCount; j++)
        {
            vector<double> solution = simpleGreatestIncrease(problemDimensionalities[i],iterations, generatedSolutionsCount, 
                neighbourhoodRange, ackleyFun, compareCondition);
            criterionFunctionVals[j] = ackleyFun(solution);
        }
        double avg = accumulate(criterionFunctionVals.begin(), criterionFunctionVals.end(), 0.0) / static_cast<double>(runsCount);
        cout << "Average criterion function for simple greatest increse algortihm with " << runsCount << " runs and problem dimensionality " << problemDimensionalities[i] << ": " << avg<<endl;
    }

    int numberOfStarts = 10;
    for (int i = 0; i < problemDimensionalities.size(); i++)
    {
        for (int j = 0; j < runsCount; j++)
        {
            //vector<double> solution = simpleGreatestIncrease(problemDimensionalities[i], iterations, generatedSolutionsCount, neighbourhoodRange, ackleyFun, compareCondition);
            vector<double> solution = multipleStartGreatestIncrease(problemDimensionalities[i], iterations, numberOfStarts ,
                generatedSolutionsCount, neighbourhoodRange, ackleyFun, compareCondition);

            criterionFunctionVals[j] = ackleyFun(solution);
        }
        double avg = accumulate(criterionFunctionVals.begin(), criterionFunctionVals.end(), 0.0) / static_cast<double>(runsCount);
        cout << "Average criterion function for multi start greatest increase algorithm with " << runsCount << " runs and problem dimensionality " << problemDimensionalities[i] << ": " << avg << endl;
    }


}

