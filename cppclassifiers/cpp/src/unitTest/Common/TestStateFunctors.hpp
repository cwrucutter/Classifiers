#pragma once
#ifndef CLASSIFIERS_BAYESIAN_TESTS_TESTSTATEFUNCTORS_HPP
#define CLASSIFIERS_BAYESIAN_TESTS_TESTSTATEFUNCTORS_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Bayesian/StateFunctor.hpp"

namespace Classifiers
{
namespace Bayesian
{
namespace Tests
{

    StateFunctorPtr MakeBooleanFunctor();

    StateFunctorPtr MakeHairColorFunctor();

    StateFunctorPtr MakeEyeColorFunctor();

} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers


#endif // end of CLASSIFIERS_BAYESIAN_TESTS_TESTSTATEFUNCTORS_HPP
