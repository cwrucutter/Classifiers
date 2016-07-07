// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "unitTest/TestFixtures/NaiveBayesClassifierTestFixtures.hpp"

namespace Classifiers
{
namespace Bayesian
{
namespace Tests
{

    TEST_F(NaiveBayesClassifierTest, TestConstructor)
    {
        this->TestNaiveBayesConstructor();
    }

    TEST_F(NaiveBayesClassifierTest, TestTrain)
    {
        this->TestNaiveBayesTrain();
    }

    TEST_F(NaiveBayesClassifierTest, TestPrOfState)
    {
        this->TestNaiveBayesPrOfState();
    }

    TEST_F(NaiveBayesClassifierTest, TestClassify)
    {
        this->TestNaiveBayesClassify();
    }

} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers
