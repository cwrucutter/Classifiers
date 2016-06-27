// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Neural/CppNeuron.hpp"

namespace Classifiers
{
namespace Neural
{

    CppNeuron::CppNeuron(double (*pFunc)(double)) :
        INeuron(), _pActivationFunction(pFunc)
    {
    }

    CppNeuron::~CppNeuron()
    {
    }

    void CppNeuron::ComputeValue()
    {
        this->_z = 0.0;
        for(SynapsePtr pSynapse : this->_incomingEdges)
        {
            this->_z += pSynapse->_pSource->GetActivation()
                * pSynapse->_weight;
        }
        this->_z += this->_bias;
        this->_a = this->_pActivationFunction(this->_z);
    }

} // end of namespace Neural
} // end of namespace Classifiers
