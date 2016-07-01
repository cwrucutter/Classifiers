// SYSTEM INCLUDES
#include <gtest/gtest.h>
#include <random>
#include <utility>
#include <vector>


// C++ PROJECT INCLUDES
#include "Neural/FFNeuralNet.hpp"
// #include "unitTest/TestFixtures/NetTestFixtures.hpp"

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    FFNeuralNetPtr                                                     pNet;
    std::vector<std::pair<std::vector<double>, std::vector<double> > > tData;
    std::vector<std::pair<std::vector<double>, std::vector<double> > > testData;

    TEST(FFNeuralNetSpeedTest, InitDataAndNet)
    {
        std::vector<int> layersScheme = {784, 30, 10};
        pNet = std::make_shared<FFNeuralNet>(layersScheme);

        // generate 50,000 784 dimensional vectors of grayscaled values
        int tDataSize = 50000;
        int testDataSize = 10000;

        tData.reserve(tDataSize);
        testData.reserve(testDataSize);
        std::default_random_engine generator;
        std::uniform_real_distribution<double> unif(0.0, 1.0);
        for(int i = 0; i < tDataSize; ++i)
        {
            std::vector<double> input;
            input.reserve(layersScheme[0]);
            std::vector<double> output;
            output.reserve(layersScheme[2]);

            for(int j = 0; j < 784; ++j)
            {
                input.push_back(unif(generator));
            }
            for(int j = 0; j < 10; ++j)
            {
                output.push_back((double)(int)(unif(generator) * 10));
            }
            tData.push_back(std::make_pair(input, output));
        }
        for(int i = 0; i < testDataSize; ++i)
        {
            std::vector<double> input;
            input.reserve(layersScheme[0]);
            std::vector<double> output;
            output.reserve(layersScheme[2]);

            for(int j = 0; j < 784; ++j)
            {
                input.push_back(unif(generator));
            }
            output.push_back((double)(int)(unif(generator) * 9));
            testData.push_back(std::make_pair(input, output));
        }
    }

    TEST(FFNeuralNetSpeedTest, FeedForwardSpeedTest)
    {
        std::vector<int> layersScheme = {784, 30, 10};
        std::default_random_engine generator;
        std::normal_distribution<double> dist(0.0, 1.0);
        std::vector<double> inputs;

        for(int i = 0; i < layersScheme[0]; ++i)
        {
            inputs.push_back(dist(generator));
        }
        pNet->FeedForward(inputs);
    }

    TEST(FFNeuralNetSpeedTest, TrainSpeedTest)
    {
        pNet->Train(tData, 1, 10, testData);
    }

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
