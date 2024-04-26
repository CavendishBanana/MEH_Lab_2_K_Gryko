// MEH_Lab_2_K_Gryko.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
// Krzysztof Gryko

#include <iostream>
#include <random>
#include <vector>
#include "IFunction.h"
#include "AckleyFunction.h"
#include "ICompare.h"
#include "CompareLess.h"
#include "INumberGenerator.h"
#include "NormalDistributionGenerator.h"
#include <algorithm>
#include <numeric>

using namespace std;

vector<double> generateNewSolution(const vector<double>& baseSolution, double neighbourhoodRange, INumberGenerator& rngNumGen)
{
    vector<double> newSol = baseSolution;
    for_each(newSol.begin(), newSol.end(), [&neighbourhoodRange, &rngNumGen](double& x) { x += neighbourhoodRange * rngNumGen.generateNumber(); });
    return newSol;
}

vector<double> simpleGreatestIncrease(int dimensionality, int iterations, int numberOfGeneratedSolutions, double neghbourhoodRange, IFunction& criterionFunction, ICompare& compareCriterion, INumberGenerator& rngNumGen)
{
    //std::default_random_engine generator;
    //std::normal_distribution<double> distribution(0.0, 1.0);
    vector<double> currentPoint = vector<double>(dimensionality, 0.0);
    for (int i = 0; i < dimensionality; i++)
    {
        currentPoint[i] = rngNumGen.generateNumber();
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
                newSolution[k] += rngNumGen.generateNumber() * neghbourhoodRange;
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


vector<double> multipleStartGreatestIncrease(int dimensionality, int iterations, int numberOfStarts, int numberOfGeneratedSolutions, 
    double neghbourhoodRange, IFunction& criterionFunction, ICompare& compareCriterion, INumberGenerator& rngNumGen)
{
    vector<double> globalSol = vector<double>(dimensionality, 0.0);
    for (int i = 0; i < globalSol.size(); i++) { globalSol[i] = rngNumGen.generateNumber(); };
    double globalCritFunVal = criterionFunction(globalSol);
    vector<vector<double>> newSols = vector<vector<double>>(numberOfGeneratedSolutions, vector<double>(dimensionality, 0.0));
    vector<double> newSolsCritFunVals = vector<double>(numberOfGeneratedSolutions, 0.0);
    for (int i = 0; i < numberOfStarts; i++)
    {
        bool local = false;
        vector<double> currentSol = vector<double>(dimensionality, 0.0);
        for (int j = 0; j < currentSol.size(); j++) { currentSol[j] = rngNumGen.generateNumber(); };
        double currentCritValFun = criterionFunction(currentSol);
        //vector<double> currentSol2 = currentSol;
        //double currentCritValFun2 = currentCritValFun;
        do {
            int bestIdxSol = -1;

            for (int j = 0; j < numberOfGeneratedSolutions; j++)
            {
                for (int k = 0; k < dimensionality; k++)
                {
                    newSols[j][k] = currentSol[k] + neghbourhoodRange * rngNumGen.generateNumber(); //
                }
                newSolsCritFunVals[j] = criterionFunction(newSols[j]);
                if (compareCriterion.leftIsBetterThanRight(currentCritValFun, newSolsCritFunVals[j])) //
                {
                    bestIdxSol = j;
                    currentCritValFun = newSolsCritFunVals[j]; //
                }
                
            }

            if (bestIdxSol != -1)
            {
                for (int k = 0; k < currentSol.size(); k++) { currentSol[k] = newSols[bestIdxSol][k]; } // //

            }
            else
            {
                local = true;
            }
            if (bestIdxSol != -1 && compareCriterion.leftIsBetterThanRight(currentCritValFun, globalCritFunVal)) //
            {
                globalCritFunVal = currentCritValFun; //
                for (int k = 0; k < globalSol.size(); k++) { globalSol[k] = newSols[bestIdxSol][k]; }
            }

        } while (!local);
    
    }
    return globalSol;
}

int main()
{   
    NormalDistributionGenerator numGen = NormalDistributionGenerator(0.0, 1.0);

    AckleyFunction ackleyFun = AckleyFunction();
    CompareLess compareCondition = CompareLess();
    
    int iterations = 1000, generatedSolutionsCount=60;
    double neighbourhoodRange = 0.2;

    vector<int> problemDimensionalities = vector<int>(2,0);
    problemDimensionalities[0] = 5;
    problemDimensionalities[1] = 8;
    int runsCount = 10;
    int numberOfStarts = 10;
    vector<double> criterionFunctionVals = vector<double>(runsCount, 0.0);

    cout << "Test simple max increase algorithm" << endl;
    for (int i = 0; i < problemDimensionalities.size(); i++)
    {
        for (int j = 0; j < runsCount; j++)
        {
            vector<double> solution = simpleGreatestIncrease(problemDimensionalities[i],iterations, generatedSolutionsCount, 
                neighbourhoodRange, ackleyFun, compareCondition, numGen);
            criterionFunctionVals[j] = ackleyFun(solution);
        }
        double avg = accumulate(criterionFunctionVals.begin(), criterionFunctionVals.end(), 0.0) / static_cast<double>(runsCount);
        cout << "Average criterion function for simple greatest increse algortihm with " << runsCount << " runs and problem dimensionality " << problemDimensionalities[i] << ": " << avg<<endl;
    }

    
    for (int i = 0; i < problemDimensionalities.size(); i++)
    {
        for (int j = 0; j < runsCount; j++)
        {
            //vector<double> solution = simpleGreatestIncrease(problemDimensionalities[i], iterations, generatedSolutionsCount, neighbourhoodRange, ackleyFun, compareCondition);
            vector<double> solution = multipleStartGreatestIncrease(problemDimensionalities[i], iterations, numberOfStarts ,
                generatedSolutionsCount, neighbourhoodRange, ackleyFun, compareCondition, numGen);

            criterionFunctionVals[j] = ackleyFun(solution);
        }
        double avg = accumulate(criterionFunctionVals.begin(), criterionFunctionVals.end(), 0.0) / static_cast<double>(runsCount);
        cout << "Average criterion function for multi start greatest increase algorithm with " << runsCount << " runs and problem dimensionality " << problemDimensionalities[i] << ": " << avg << endl;
    }


}

