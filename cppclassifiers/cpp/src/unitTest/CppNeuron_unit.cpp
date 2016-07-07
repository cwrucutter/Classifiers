// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "unitTest/TestFixtures/NeuronTestFixtures.hpp"


namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    TEST_F(NeuronTest, TestNeuronConstructor)
    {
        this->TestConstructor();
    }

    TEST_F(NeuronTest, TestNeuronComputeValue)
    {
        this->TestComputeValue();
    }

    TEST_F(NeuronTest, TestNeuronUpdate)
    {
        this->TestUpdate();
    }

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
