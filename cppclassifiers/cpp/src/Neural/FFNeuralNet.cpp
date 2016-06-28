// SYSTEM INCLUDES
#include <math.h>

// C++ PROJECT INCLUDES
#include "Neural/FFNeuralNet.hpp"
#include "Neural/CppNeuron.hpp"

namespace Classifiers
{
namespace Neural
{

    double Sigmoid(double z)
    {
        return 1.0 / (1.0 + exp(-1 * z));
    }

    double SigmoidPrime(double z)
    {
        double val = Sigmoid(z);
        return val * (1 - pow(val, 2.0));
    }

    FFNeuralNet::FFNeuralNet(std::vector<int> layersConfig,
                             double learningRate) : _numLayers(layersConfig.size()),
        _learningRate(learningRate), _layers()
    {
        SynapsePtr pSynapse = nullptr;
        for(int i = 0; i < this->_numLayers; ++i)
        {
            std::vector<INeuronPtr> layer;
            for(int j = 0; j < (int)layersConfig[i]; ++j)
            {
                layer.push_back(std::make_shared<CppNeuron>(&Sigmoid));
            }
            this->_layers.push_back(layer);
        }

        for(int i = 0; i < this->_numLayers - 1; ++i)
        {
            for(INeuronPtr pNeuron1 : this->_layers[i])
            {
                for(INeuronPtr pNeuron2 : this->_layers[i + 1])
                {
                    pSynapse = std::make_shared<Synapse>(pNeuron1, pNeuron2);
                    pNeuron1->_outgoingEdges.push_back(pSynapse);
                    pNeuron2->_incomingEdges.push_back(pSynapse);
                }
            }
        }
    }

    FFNeuralNet::~FFNeuralNet()
    {
    }

    std::vector<double> FFNeuralNet::FeedForward(std::vector<double> inputs)
    {
        int inputIndex = 0;
        std::vector<double> result;
        for(int layerIndex = 0; layerIndex < this->_numLayers; ++layerIndex)
        {
            if(layerIndex == 0)
            {
                for(INeuronPtr pInputNeuron : this->_layers[layerIndex])
                {
                    pInputNeuron->_a = inputs[inputIndex];
                    ++inputIndex;
                }
            }
            else if(layerIndex == this->_numLayers - 1)
            {
                for(INeuronPtr pOutputNeuron : this->_layers[layerIndex])
                {
                    pOutputNeuron->ComputeValue();
                    result.push_back(pOutputNeuron->_a);
                }
            }
            else
            {
                for(INeuronPtr pNeuron : this->_layers[layerIndex])
                {
                    pNeuron->ComputeValue();
                }
            }
        }
        return result;
    }

} // end of namespace Neural
} // end of namespace Classifiers
