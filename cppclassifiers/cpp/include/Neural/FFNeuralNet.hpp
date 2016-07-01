#pragma once
#ifndef CLASSIFIERS_NEURAL_FFNEURALNET_HPP
#define CLASSIFIERS_NEURAL_FFNEURALNET_HPP

#ifndef TEST_NET_FRIENDS
#define TEST_NET_FRIENDS
#endif


// SYSTEM INCLUDES
#include <list>
#include <memory>
#include <utility>
#include <vector>

// C++ PROJECT INCLUDES
#include "Neural/INeuron.hpp"

namespace Classifiers
{
namespace Neural
{

    class FFNeuralNet;

    double Sigmoid(double z);

    double SigmoidPrime(double z);

    double DefaultCostFunction(std::vector<double> actual,
                               std::vector<double> expected);

    double DefaultCostFunctionPrime(double actual, double expected);

    int DefaultEvalFunc(std::vector<std::pair<std::vector<double>,
                                              std::vector<double> > > evalData,
                        FFNeuralNet* pNet);

    class FFNeuralNet
    {
        TEST_NET_FRIENDS;
        public:

            FFNeuralNet(std::vector<int> layersConfig,
                        double learningRate=1.0,
                        double (*costFunc)(std::vector<double>,
                                           std::vector<double>)=&DefaultCostFunction,
                        double (*costFuncPrime)(double, double)=&DefaultCostFunctionPrime,
                        int (*evaluationFunction)(
                                std::vector<std::pair<std::vector<double>,
                                                      std::vector<double> > >,
                                FFNeuralNet*)=&DefaultEvalFunc);

            ~FFNeuralNet();

            std::vector<double> FeedForward(std::vector<double> inputs);

            void Train(std::vector<std::pair<std::vector<double>,
                                             std::vector<double> > > trainingData,
                       int epochs, int miniBatchSize,
                       std::vector<std::pair<std::vector<double>,
                                             std::vector<double> > > testData={});

        private:

            void BackPropogate(std::vector<double> inputs,
                               std::vector<double> expectedOutputs);

            void MiniBatchUpdate(std::list<std::pair<std::vector<double>,
                                                       std::vector<double> > > miniBatch,
                                 int miniBatchSize);

            int                                     _numLayers;
            double                                  _learningRate;
            std::vector<std::vector<INeuronPtr> >   _layers;
            double (*_costFunction)(std::vector<double>, std::vector<double>);
            double (*_costFunctionPrime)(double, double);
            int (*_evaluationFunction)(std::vector<std::pair<std::vector<double>,
                                                                std::vector<double> > >,
                                          FFNeuralNet* pNet);

    };

    using FFNeuralNetPtr = std::shared_ptr<FFNeuralNet>;

} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_FFNEURALNET_HPP
