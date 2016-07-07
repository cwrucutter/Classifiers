#pragma once
#ifndef CLASSIFIERS_NEURAL_NEURON_HPP
#define CLASSIFIERS_NEURAL_NEURON_HPP

#ifndef TEST_NEURON_FRIENDS
#define TEST_NEURON_FRIENDS
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

    class Neuron
    {
        friend class FFNeuralNet;
        TEST_NEURON_FRIENDS;
        public:

            Neuron(double (*ActiveFunc)(double),
                    double (*ActiveFuncPrime)(double));

            ~Neuron();

            double& ComputeValue();

            double& ComputeValue(std::vector<double>& input);

            void Update(double learningRate);

            double _a;

        protected:

            double _z, _delta, _deltaUpdate, _bias;

            std::vector<SynapsePtr> _incomingEdges;
            //std::vector<SynapsePtr> _outgoingEdges;

        public:

            double (*ActivationFunction)(double);
            double (*ActivationFunctionPrime)(double);

    };

    using NeuronPtr = std::shared_ptr<Neuron>;

} // end of namespace Neural
} // end of namespace Classifiers


#endif // end of CLASSIFIERS_NEURAL_NEURON_HPP