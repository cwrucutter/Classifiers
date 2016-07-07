#pragma once
#ifndef CLASSIFIERS_BAYESIAN_TESTS_NAIVEBAYESCLASSIFIERTESTFIXTURES_HPP
#define CLASSIFIERS_BAYESIAN_TESTS_NAIVEBAYESCLASSIFIERTESTFIXTURES_HPP

// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "unitTest/Common/FriendDeclarations.hpp"
#include "Bayesian/NaiveBayesClassifier.hpp"

namespace Classifiers
{
namespace Bayesian
{
namespace Tests
{

    class NaiveBayesClassifierTest : public ::testing::Test
    {
        protected:

            virtual void SetUp();

            virtual void TearDown();

            void TestNaiveBayesConstructor();

            void TestNaiveBayesTrain();

            void TestNaiveBayesPrOfState();

            void TestNaiveBayesClassify();

    };

} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers


#endif // end of CLASSIFIERS_BAYESIAN_TESTS_NAIVEBAYESCLASSIFIERSTESTFIXTURES_HPP
