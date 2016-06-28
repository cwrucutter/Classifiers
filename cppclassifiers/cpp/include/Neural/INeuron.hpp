#pragma once
#ifndef CLASSIFIERS_NEURAL_INEURON_HPP
#define CLASSIFIERS_NEURAL_INEURON_HPP

#ifndef TEST_CPPNEURON_FRIENDS
#define TEST_CPPNEURON_FRIENDS
#endif


// SYSTEM INCLUDES
#include <memory>
#include <vector>

// C++ PROJECT INCLUDES
#include "Neural/Synapse.hpp"

namespace Classifiers
{
namespace Neural
{

    class FFNeuralNet;

    class INeuron
    {
        friend class FFNeuralNet;
        TEST_CPPNEURON_FRIENDS;
        public:

            INeuron();

            virtual ~INeuron();

            virtual void ComputeValue() = 0;

            void Update(double learningRate);

            double GetActivation();

        protected:

            double _a, _z, _delta, _deltaUpdate, _bias;

            std::vector<SynapsePtr> _incomingEdges;
            std::vector<SynapsePtr> _outgoingEdges;

    };

    using INeuronPtr = std::shared_ptr<INeuron>;

} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_INEURON_HPP
