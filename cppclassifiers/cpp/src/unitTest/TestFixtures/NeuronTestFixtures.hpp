#pragma once
#ifndef CLASSIFIERS_NEURAL_NEURONTESTFIXTURES_HPP
#define CLASSIFIERS_NEURAL_NEURONTESTFIXTURES_HPP

// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "unitTest/Common/FriendDeclarations.hpp"
#include "Neural/INeuron.hpp"
#include "Neural/CppNeuron.hpp"

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    class CppNeuronTest : public ::testing::Test
    {
        protected:

            CppNeuronPtr _pNeuron;

            virtual void SetUp();

            virtual void TearDown();

            void TestConstructor();

            void TestComputeValue();

            void TestUpdate();

    };

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_NEURONTEXTFIXTURES_HPP
