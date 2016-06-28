#pragma once
#ifndef CLASSIFIERS_NEURAL_FFNEURALNET_HPP
#define CLASSIFIERS_NEURAL_FFNEURALNET_HPP

#ifndef TEST_NET_FRIENDS
#define TEST_NET_FRIENDS
#endif


// SYSTEM INCLUDES
#include <memory>
#include <vector>

// C++ PROJECT INCLUDES
#include "Neural/INeuron.hpp"

namespace Classifiers
{
namespace Neural
{

    double Sigmoid(double z);

    double SigmoidPrime(double z);

    class FFNeuralNet
    {
        TEST_NET_FRIENDS;
        public:

            FFNeuralNet(std::vector<int> layersConfig,
                        double learningRate=1.0);

            ~FFNeuralNet();

            std::vector<double> FeedForward(std::vector<double> inputs);

        private:

            int                                     _numLayers;
            double                                  _learningRate;
            std::vector<std::vector<INeuronPtr> >   _layers;

    };

    using FFNeuralNetPtr = std::shared_ptr<FFNeuralNet>;

} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_FFNEURALNET_HPP
