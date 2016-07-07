// SYSTEM INCLUDES
#include <memory>
#include <vector>

// C++ PROJECT INCLUDES
#include "unitTest/Common/TestStateFunctors.hpp"

namespace Classifiers
{
namespace Bayesian
{
namespace Tests
{

    StateFunctorPtr MakeBooleanFunctor()
    {
        return std::make_shared<StateFunctor>(std::vector<std::string>{"false", "true"});
    }

    StateFunctorPtr MakeHairColorFunctor()
    {
        return std::make_shared<StateFunctor>(std::vector<std::string>{"brown", "blonde",
                                               "black"});
    }

    StateFunctorPtr MakeEyeColorFunctor()
    {
        return std::make_shared<StateFunctor>(std::vector<std::string>{"brown", "blue",
                                               "black", "green"});
    }

} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers
