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

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
