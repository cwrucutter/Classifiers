// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "unitTest/TestFixtures/NetTestFixtures.hpp"

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    TEST_F(FFNeuralNetTest, TestNetConstructor)
    {
        this->TestFFNetConstructor();
    }

    TEST_F(FFNeuralNetTest, TestNetFeedForward)
    {
        this->TestFFNetFeedForward();
    }

    TEST_F(FFNeuralNetTest, TestNetDefaultEvaluationFunction)
    {
        this->TestFFNetDefaultEvaluationFunction();
    }

    TEST_F(FFNeuralNetTest, TestNetBackPropogate)
    {
        this->TestFFNetBackPropogate();
    }

    TEST_F(FFNeuralNetTest, TestNetMiniBatchUpdate)
    {
        this->TestFFNetMiniBatchUpdate();
    }

    TEST_F(FFNeuralNetTest, TestNetMiniBatchPartitionAlgorithm)
    {
        this->TestFFNetMiniBatchPartitionAlgorithm();
    }

    TEST_F(FFNeuralNetTest, TestNetTrain)
    {
        this->TestFFNetTrain();
    }

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
