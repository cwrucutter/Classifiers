// SYSTEM INCLUDES
#include <math.h>

// C++ PROJECT INCLUDES
#include "unitTest/Common/TestFunctions.hpp"

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    double Sigmoid(double z)
    {
        return 1.0 / (1.0 + exp(-1 * z));
    }

    double SigmoidPrime(double z)
    {
        double val = Sigmoid(z);
        return val * (1 - pow(val, 2.0));
    }

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
