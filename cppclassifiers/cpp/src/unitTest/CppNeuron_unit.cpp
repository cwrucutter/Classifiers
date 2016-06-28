// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "unitTest/TestFixtures/NeuronTestFixtures.hpp"


namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    TEST_F(CppNeuronTest, TestNeuronConstructor)
    {
        this->TestConstructor();
    }

    TEST_F(CppNeuronTest, TestNeuronComputeValue)
    {
        this->TestComputeValue();
    }

    TEST_F(CppNeuronTest, TestNeuronUpdate)
    {
        this->TestUpdate();
    }

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
