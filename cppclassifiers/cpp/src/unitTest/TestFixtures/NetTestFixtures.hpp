#pragma once
#ifndef CLASSIFIERS_NEURAL_NETTESTFIXTURES_HPP
#define CLASSIFIERS_NEURAL_NETTESTFIXTURES_HPP

// SYSTEM INCLUDES
#include <gtest/gtest.h>
#include <map>
#include <memory>
#include <string>

// C++ PROJECT INCLUDES
#include "unitTest/Common/FriendDeclarations.hpp"
#include "Neural/Neuron.hpp"
#include "Neural/FFNeuralNet.hpp"

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    class NetTestNeuron
    {
        public:

            NetTestNeuron();

            ~NetTestNeuron();

            void SetNeuronParams(NeuronPtr pNeuron,
                std::map<std::string, double> dict);

            bool CheckSynapse(NeuronPtr getSynapseFrom, int index,
                NeuronPtr pSource, NeuronPtr pDest);

            int GetLengthOfEdges(NeuronPtr pNeuron);

            double GetDeltaUpdate(NeuronPtr pNeuron);

            double GetBias(NeuronPtr pNeuron);

            std::vector<SynapsePtr> GetEdges(NeuronPtr pNeuron);

    };

    using NetTestNeuronPtr = std::shared_ptr<NetTestNeuron>;

    class FFNeuralNetTest : public ::testing::Test
    {
        protected:

            virtual void SetUp();

            virtual void TearDown();

            void CheckGraphStructure(FFNeuralNetPtr pNet, std::vector<int> layerScheme);

            void SetupGraph(FFNeuralNetPtr pNet, std::map<std::string, double> params);

            void TestFFNetConstructor();

            void TestFFNetFeedForward();

            void TestFFNetBackPropogate();

            void TestFFNetMiniBatchUpdate();

            void TestFFNetMiniBatchPartitionAlgorithm();

            void TestFFNetTrain();

            void TestFFNetDefaultEvaluationFunction();

            FFNeuralNetPtr      _pNet;
            NetTestNeuronPtr    _pNeuronChanger;
    };

    /**
    class FFNeuralNetSpeedTest : public ::testing::Test
    {
        protected:

            virtual void SetUp();

            virtual void TearDown();

            void FFNetFeedForwardSpeedTest();

            void FFNetTrainSpeedTest();

            FFNeuralNetPtr      _pNet;
            std::vector<std::pair<std::vector<double>, std::vector<double> > > _tData;
            std::vector<std::pair<std::vector<double>, std::vector<double> > > _testData;
    };
    */

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_NETTESTFIXTURES_HPP
