// SYSTEM INCLUDES
#include <algorithm> // std::random_shuffle
#include <math.h>    // exp, pow

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

    double DefaultCostFunction(std::vector<double> actual,
                               std::vector<double> expected)
    {
        double error = 0.0;
        for(int i = 0; i < (int)actual.size(); ++i)
        {
            error += pow(actual[i] - expected[i], 2.0);
        }
        return 0.5 * error;
    }

    double DefaultCostFunctionPrime(double actual, double expected)
    {
        return actual - expected;
    }

    int DefaultEvalFunc(std::vector<std::pair<std::vector<double>,
                                              std::vector<double> > > evalData,
                        FFNeuralNet* pNet)
    {
        using Tuple_t = std::pair<std::vector<double>, std::vector<double> >;
        int correctClassification = 0;
        std::vector<double> feedForwardResults;
        for(Tuple_t tuple : evalData)
        {
            feedForwardResults = pNet->FeedForward(std::get<0>(tuple));
            correctClassification += (std::distance(feedForwardResults.begin(),
                                                    std::max_element(feedForwardResults.begin(),
                                                                     feedForwardResults.end())
                                       ) == std::get<1>(tuple)[0]);
        }
        return correctClassification;
    }

    FFNeuralNet::FFNeuralNet(std::vector<int> layersConfig,
                             double learningRate, double (*costFunc)(std::vector<double>,
                                                                     std::vector<double>),
                             double (*costFuncPrime)(double, double),
                             int (*evaluationFunction)(std::vector<std::pair<std::vector<double>,
                                                                                 std::vector<double> > >,
                                                          FFNeuralNet*)) :
        _numLayers(layersConfig.size()),
        _learningRate(learningRate), _layers(), _costFunction(costFunc),
        _costFunctionPrime(costFuncPrime), _evaluationFunction(evaluationFunction)
    {
        SynapsePtr pSynapse = nullptr;
        for(int i = 0; i < this->_numLayers; ++i)
        {
            std::vector<INeuronPtr> layer;
            for(int j = 0; j < (int)layersConfig[i]; ++j)
            {
                layer.push_back(std::make_shared<CppNeuron>(&Sigmoid, &SigmoidPrime));
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

    std::vector<double> FFNeuralNet::FeedForward(std::vector<double>& inputs)
    {
        int inputIndex = 0;
        std::vector<double> result;
        for(int layerIndex = 0; layerIndex < this->_numLayers; ++layerIndex)
        {
            if(layerIndex == 0)
            {
                for(INeuronPtr& pInputNeuron : this->_layers[layerIndex])
                {
                    pInputNeuron->_a = inputs[inputIndex];
                    ++inputIndex;
                }
            }
            else if(layerIndex == this->_numLayers - 1)
            {
                for(INeuronPtr& pOutputNeuron : this->_layers[layerIndex])
                {
                    pOutputNeuron->ComputeValue();
                    result.push_back(pOutputNeuron->_a);
                }
            }
            else
            {
                for(INeuronPtr& pNeuron : this->_layers[layerIndex])
                {
                    pNeuron->ComputeValue();
                }
            }
        }
        return result;
    }

    void FFNeuralNet::Train(std::vector<std::pair<std::vector<double>,
                                                  std::vector<double> > >& trainingData,
                            int epochs, int miniBatchSize,
                            std::vector<std::pair<std::vector<double>,
                                                  std::vector<double> > >& testData)
    {
        //using MiniBatch_t = std::vector<std::pair<std::vector<double>, std::vector<double> > >;
        int testDataLength = 0;
        
        if(testData.size() > 0)
        {
            testDataLength = (int)testData.size();
        }
        int n = (int)trainingData.size();
        for(int i = 0; i < epochs; ++i)
        {
            //std::vector<MiniBatch_t> miniBatches;
            //miniBatches.reserve(n / miniBatchSize);
            // shuffle trainingData
            std::random_shuffle(trainingData.begin(), trainingData.end());
            // group into list of mini batches
            for(int k = 0; k < n; k += miniBatchSize)
            {
                //MiniBatch_t miniBatch;
                //for(int l = k; l < k + miniBatchSize; ++l)
                //{
                //    miniBatch.push_back(trainingData[l]);
                //}
                //miniBatches.push_back(miniBatch);
                this->MiniBatchUpdate(trainingData, k, miniBatchSize);
            }
            //for(MiniBatch_t& miniBatch : miniBatches)
            //{
            //    this->MiniBatchUpdate(miniBatch, miniBatchSize);
            //}
        }
    }

    void FFNeuralNet::Train(std::vector<std::pair<std::vector<double>,
                                                  std::vector<double> > >& trainingData,
                            int epochs, int miniBatchSize)
    {
        //using MiniBatch_t = std::vector<std::pair<std::vector<double>, std::vector<double> > >;
        
        int n = (int)trainingData.size();
        for(int i = 0; i < epochs; ++i)
        {
            //std::vector<MiniBatch_t> miniBatches;
            //miniBatches.reserve(n / miniBatchSize);
            // shuffle trainingData
            std::random_shuffle(trainingData.begin(), trainingData.end());
            // group into list of mini batches
            for(int k = 0; k < n; k += miniBatchSize)
            {
                //MiniBatch_t miniBatch;
                //for(int l = k; l < k + miniBatchSize; ++l)
                //{
                //    miniBatch.push_back(trainingData[l]);
                //}
                //miniBatches.push_back(miniBatch);
                this->MiniBatchUpdate(trainingData, k, miniBatchSize);
            }
            //for(MiniBatch_t& miniBatch : miniBatches)
            //{
            //    this->MiniBatchUpdate(miniBatch, miniBatchSize);
            //}
        }
    }

    void FFNeuralNet::BackPropogate(std::vector<double>& inputs,
                                    std::vector<double>& expectedOutputs)
    {
        this->FeedForward(inputs);
        int expectedOutputIndex = 0;
        for(int i = this->_numLayers - 1; i > 0; --i)
        {
            if(i == this->_numLayers - 1)
            {
                for(INeuronPtr& pOutputNeuron : this->_layers[i])
                {
                    pOutputNeuron->_delta =
                        this->_costFunctionPrime(pOutputNeuron->_a,
                                                 expectedOutputs[expectedOutputIndex])
                        * pOutputNeuron->ActivationFunctionPrime(pOutputNeuron->_z);
                    pOutputNeuron->_deltaUpdate += pOutputNeuron->_delta;

                    for(SynapsePtr& pSynapse : pOutputNeuron->_incomingEdges)
                    {
                        pSynapse->_wUpdate += pSynapse->_pSource->_a * pOutputNeuron->_delta;
                    }
                    ++expectedOutputIndex;
                }
            }
            else
            {
                for(INeuronPtr& pNeuron : this->_layers[i])
                {
                    pNeuron->_delta = 0.0;
                    for(SynapsePtr& pSynapse : pNeuron->_outgoingEdges)
                    {
                        pNeuron->_delta += pSynapse->_weight * pSynapse->_pDest->_delta;
                    }
                    pNeuron->_delta *= pNeuron->ActivationFunctionPrime(pNeuron->_z);
                    pNeuron->_deltaUpdate += pNeuron->_delta;

                    for(SynapsePtr& pSynapse : pNeuron->_incomingEdges)
                    {
                        pSynapse->_wUpdate += pSynapse->_pSource->_a * pNeuron->_delta;
                    }
                }
            }
        }
    }

    /**
    void FFNeuralNet::MiniBatchUpdate(std::vector<std::pair<std::vector<double>,
                                                            std::vector<double> > >& miniBatch,
                                      int miniBatchSize)
    {
        using MiniBatchTuple_t = std::pair<std::vector<double>, std::vector<double> >;
        for(MiniBatchTuple_t& tuple : miniBatch)
        {
            this->BackPropogate(std::get<0>(tuple), std::get<1>(tuple));
        }
        for(int i = this->_numLayers - 1; i >= 0; --i)
        {
            for(INeuronPtr& pNeuron : this->_layers[i])
            {
                for(SynapsePtr& pSynapse : pNeuron->_incomingEdges)
                {
                    pSynapse->Update(this->_learningRate / miniBatchSize);
                }
                pNeuron->Update(this->_learningRate / miniBatchSize);
            }
        }
    }
    */

    void FFNeuralNet::MiniBatchUpdate(std::vector<std::pair<std::vector<double>,
                                                            std::vector<double> > >& trainingData,
                                      int startIndex, int miniBatchSize)
    {
        //using MiniBatchTuple_t = std::pair<std::vector<double>, std::vector<double> >;
        //for(MiniBatchTuple_t& tuple : miniBatch)
        //{
        //    this->BackPropogate(std::get<0>(tuple), std::get<1>(tuple));
        //}
        for(int i = 0; i < miniBatchSize; ++i)
        {
            this->BackPropogate(std::get<0>(trainingData[i + startIndex]),
                                std::get<1>(trainingData[i + startIndex]));
        }

        for(int i = this->_numLayers - 1; i >= 0; --i)
        {
            for(INeuronPtr& pNeuron : this->_layers[i])
            {
                for(SynapsePtr& pSynapse : pNeuron->_incomingEdges)
                {
                    pSynapse->Update(this->_learningRate / miniBatchSize);
                }
                pNeuron->Update(this->_learningRate / miniBatchSize);
            }
        }
    }

} // end of namespace Neural
} // end of namespace Classifiers
