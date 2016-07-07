// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "unitTest/TestFixtures/BayesianNodeTestFixtures.hpp"

namespace Classifiers
{
namespace Bayesian
{
namespace Tests
{

    TEST_F(BayesianNodeTest, TestConstructor)
    {
        this->TestNodeConstructor();
    }

    TEST_F(BayesianNodeTest, TestValuesToIndex)
    {
        this->TestNodeValuesToIndex();
    }

    TEST_F(BayesianNodeTest, TestAccessTable)
    {
        this->TestNodeAccessTable();
    }

} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers
