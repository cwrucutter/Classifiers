#pragma once
#ifndef CLASSIFIERS_BAYESIAN_TESTS_BAYESIANNODETESTFIXTURES_HPP
#define CLASSIFIERS_BAYESIAN_TESTS_BAYESIANNODETESTFIXTURES_HPP

// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "unitTest/Common/FriendDeclarations.hpp"
#include "Bayesian/BayesianNode.hpp"

namespace Classifiers
{
namespace Bayesian
{
namespace Tests
{

    class BayesianNodeTest : public ::testing::Test
    {
        protected:

            virtual void SetUp();

            virtual void TearDown();

            void TestNodeConstructor();

            void TestNodeValuesToIndex();

            void TestNodeAccessTable();

            void TestNodePrOfState();

    };

} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_BAYESIAN_TESTS_BAYESIANNODETESTFIXTURES_HPP