// SYSTEM INCLUDES
#include <gtest/gtest.h>
#include <random>
#include <vector>

// C++ PROJECT INCLUDES
#include "Neural/FFNeuralNet.hpp"

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    TEST(NeuralNetSpeedTest, FeedForwardSpeedTests)
    {
        std::vector<int> layersScheme = {784, 30, 10};
        FFNeuralNet net(layersScheme);

        std::default_random_engine generator;
        std::normal_distribution<double> dist(0.0, 1.0);
        std::vector<double> inputs;

        for(int i = 0; i < layersScheme[0]; ++i)
        {
            inputs.push_back(dist(generator));
        }
        net.FeedForward(inputs);
    }

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
