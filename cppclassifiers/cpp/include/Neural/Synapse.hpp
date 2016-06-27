#pragma once
#ifndef CLASSIFIERS_NEURAL_SYNAPSE_HPP
#define CLASSIFIERS_NEURAL_SYNAPSE_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES


namespace Classifiers
{
namespace Neural
{

    class INeuron;


    struct Synapse
    {
        std::shared_ptr<INeuron> _pSource, _pDest;
        double _weight, _wUpdate;

        Synapse(std::shared_ptr<INeuron> pSource,
                std::shared_ptr<INeuron> pDest);

        ~Synapse();

        void Update(double learningRate);
    };

    using SynapsePtr = std::shared_ptr<Synapse>;

} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_SYNAPSE_HPP
