// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Neural/CppNeuron.hpp"

namespace Classifiers
{
namespace Neural
{

    CppNeuron::CppNeuron(double (*pActiveFunc)(double),
                         double (*pActiveFuncPrime)(double)) :
        INeuron(), _pActivationFunction(pActiveFunc),
        _pActivationFunctionPrime(pActiveFuncPrime)
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

    double CppNeuron::ActivationFunction(double val)
    {
        return this->_pActivationFunction(val);
    }

    double CppNeuron::ActivationFunctionPrime(double val)
    {
        return this->_pActivationFunctionPrime(val);
    }


} // end of namespace Neural
} // end of namespace Classifiers
