// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Neural/CppNeuron.hpp"
#include "unitTest/TestFunctions.hpp"

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    TEST(CppNeuronTest, TestConstructor)
    {
        CppNeuron testNeuron(&Sigmoid);
        ASSERT_NE(nullptr, &testNeuron);
        ASSERT_EQ(0.0, testNeuron.GetActivation());
    }

    TEST(CppNeuronTest, TestComputeValue)
    {
        
    }


} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
