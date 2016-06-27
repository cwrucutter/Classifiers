// SYSTEM INCLUDES
#include <random>

// C++ PROJECT INCLUDES
#include "Neural/INeuron.hpp"

namespace Classifiers
{
namespace Neural
{

    INeuron::INeuron() : _a(0.0), _z(0.0),
        _delta(0.0), _deltaUpdate(0.0),
        _bias(0.0 /**make random value*/),
        _incomingEdges(), _outgoingEdges()
    {
        std::default_random_engine generator;
        std::normal_distribution<double> dist(0.0, 1.0);
        this->_bias = dist(generator);
    }

    INeuron::~INeuron()
    {
    }

    void INeuron::Update(double learningRate)
    {
        this->_bias -= learningRate * this->_deltaUpdate;
        this->_deltaUpdate = 0.0;
    }

    double INeuron::GetActivation()
    {
        return this->_a;
    }

    double INeuron::GetBias()
    {
        return this->_bias;
    }

} // end of namespace Neural
} // end of namespace Classifiers
