// SYSTEM INCLUDES
#include <random>

// C++ PROJECT INCLUDES
#include "Neural/Neuron.hpp"

namespace Classifiers
{
namespace Neural
{

    Neuron::Neuron(double (*ActiveFunc)(double),
                    double (*ActiveFuncPrime)(double)) : _a(0.0), _z(0.0),
        _delta(0.0), _deltaUpdate(0.0),
        _bias(0.0 /**make random value*/),
        _incomingEdges(), //_outgoingEdges(),
        ActivationFunction(ActiveFunc), ActivationFunctionPrime(ActiveFuncPrime)
    {
        std::default_random_engine generator;
        std::normal_distribution<double> dist(0.0, 1.0);
        this->_bias = dist(generator);
    }

    Neuron::~Neuron()
    {
    }

    double& Neuron::ComputeValue()
    {
        this->_z = this->_bias;
        for(SynapsePtr& pSynapse : this->_incomingEdges)
        {
            this->_z += pSynapse->_pSource->_a
                * pSynapse->_weight;
        }
        //this->_z += this->_bias;
        this->_a = this->ActivationFunction(this->_z);
        return this->_a;
    }

    double& Neuron::ComputeValue(std::vector<double>& input)
    {
        this->_z = this->_bias;
        for(int i = 0; i < input.size(); ++i)
        {
            // this->_incomingEdges[i]->_pSource->_a = input[i];
            this->_z += input[i] * this->_incomingEdges[i]->_weight;
        }
        //this->_z += this->_bias;
        this->_a = this->ActivationFunction(this->_z);
        return this->_a;
    }

    void Neuron::Update(double learningRate)
    {
        this->_bias -= learningRate * this->_deltaUpdate;
        this->_deltaUpdate = 0.0;
    }

} // end of namespace Neural
} // end of namespace Classifiers
