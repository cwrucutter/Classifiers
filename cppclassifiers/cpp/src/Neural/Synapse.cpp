// SYSTEM INCLUDES
#include <random>

// C++ PROJECT INCLUDES
#include "Neural/Synapse.hpp"
#include "Neural/INeuron.hpp"

namespace Classifiers
{
namespace Neural
{

    Synapse::Synapse(INeuronPtr pSource,
                     INeuronPtr pDest) : _pSource(pSource),
        _pDest(pDest), _weight(0.0 /**make random value*/),
        _wUpdate(0.0)
    {
        std::default_random_engine generator;
        std::normal_distribution<double> dist(0.0, 1.0);
        this->_weight = dist(generator);
    }

    Synapse::~Synapse()
    {
    }

    void Synapse::Update(double learningRate)
    {
        this->_weight -= learningRate * this->_wUpdate;
        this->_wUpdate = 0.0;
    }

} // end of namespace Neural
} // end of namespace Classifiers
