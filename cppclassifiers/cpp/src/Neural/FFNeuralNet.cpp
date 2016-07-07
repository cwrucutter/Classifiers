// SYSTEM INCLUDES
#include <algorithm> // std::random_shuffle
#include <math.h>    // exp, pow
#include <mutex>
#include <thread>

// C++ PROJECT INCLUDES
#include "Neural/FFNeuralNet.hpp"

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
            std::vector<NeuronPtr> layer;
            for(int j = 0; j < (int)layersConfig[i]; ++j)
            {
                layer.push_back(std::make_shared<Neuron>(&Sigmoid, &SigmoidPrime));
            }
            this->_layers.push_back(layer);
        }

        for(int i = 0; i < this->_numLayers - 1; ++i)
        {
            for(NeuronPtr pNeuron1 : this->_layers[i])
            {
                for(NeuronPtr pNeuron2 : this->_layers[i + 1])
                {
                    pSynapse = std::make_shared<Synapse>(pNeuron1, pNeuron2);
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
        int lastLayerIndex = this->_numLayers - 1;
        std::vector<double> result(this->_layers[lastLayerIndex].size(), 0.0);
        //result.reserve(this->_layers[this->_numLayers - 1].size());
        //int inputIndex = 0;
        for(NeuronPtr& pInputNeuron : this->_layers[1])
        {
            //pInputNeuron->_a = inputs[inputIndex];
            //++inputIndex;
            pInputNeuron->ComputeValue(inputs);
        }
        for(int layerIndex = 2/**0*/; layerIndex < lastLayerIndex; ++layerIndex)
        {
            for(NeuronPtr& pNeuron : this->_layers[layerIndex])
            {
                pNeuron->ComputeValue();
            }
        }
        for(int i = 0; i < this->_layers[lastLayerIndex].size(); ++i)
        {
            result[i] = this->_layers[lastLayerIndex][i]->ComputeValue();
        }
        //for(INeuronPtr& pOutputNeuron : this->_layers[layerIndex])
        //{
            //result.push_back(pOutputNeuron->ComputeValue());
            // result.push_back(pOutputNeuron->_a);
        //}
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

    void FFNeuralNet::FeedForwardNoResult(std::vector<double>& inputs)
    {
        for(NeuronPtr& pInputNeuron : this->_layers[1])
        {
            //pInputNeuron->_a = inputs[inputIndex];
            //++inputIndex;
            pInputNeuron->ComputeValue(inputs);
        }
        for(int layerIndex = 2/**0*/; layerIndex < this->_numLayers; ++layerIndex)
        {
            for(NeuronPtr& pNeuron : this->_layers[layerIndex])
            {
                pNeuron->ComputeValue();
            }
        }
    }

    void FFNeuralNet::BackPropogate(std::vector<double>& inputs,
                                    std::vector<double>& expectedOutputs)
    {
        this->FeedForwardNoResult(inputs);
        int expectedOutputIndex = 0;
        for(int i = this->_numLayers - 1; i > 0; --i)
        {
            if(i == this->_numLayers - 1)
            {
                for(NeuronPtr& pOutputNeuron : this->_layers[i])
                {
                    pOutputNeuron->_delta =
                        this->_costFunctionPrime(pOutputNeuron->_a,
                                                 expectedOutputs[expectedOutputIndex])
                        * pOutputNeuron->ActivationFunctionPrime(pOutputNeuron->_z);
                    pOutputNeuron->_deltaUpdate += pOutputNeuron->_delta;

                    for(SynapsePtr& pSynapse : pOutputNeuron->_incomingEdges)
                    {
                        pSynapse->_pSource->_delta += pSynapse->_weight * pOutputNeuron->_delta;
                        pSynapse->_wUpdate += pSynapse->_pSource->_a * pOutputNeuron->_delta;
                    }
                    ++expectedOutputIndex;
                }
            }
            else if(i == 1)
            {
                for(NeuronPtr& pNeuron : this->_layers[i])
                {
                    //pNeuron->_delta = 0.0;
                    //for(SynapsePtr& pSynapse : pNeuron->_outgoingEdges)
                    //{
                    //    pNeuron->_delta += pSynapse->_weight * pSynapse->_pDest->_delta;
                    //}
                    pNeuron->_delta *= pNeuron->ActivationFunctionPrime(pNeuron->_z);
                    pNeuron->_deltaUpdate += pNeuron->_delta;

                    for(int j = 0; j < pNeuron->_incomingEdges.size(); ++j)
                    {
                        pNeuron->_incomingEdges[j]->_wUpdate += inputs[j] * pNeuron->_delta;
                    }
                    pNeuron->_delta = 0.0;
                    //for(SynapsePtr& pSynapse : pNeuron->_incomingEdges)
                    //{
                    //    pSynapse->_wUpdate += pSynapse->_pSource->_a * pNeuron->_delta;
                    //}
                }
            }
            else
            {
                for(NeuronPtr& pNeuron : this->_layers[i])
                {
                    //pNeuron->_delta = 0.0;
                    //for(SynapsePtr& pSynapse : pNeuron->_outgoingEdges)
                    //{
                    //    pNeuron->_delta += pSynapse->_weight * pSynapse->_pDest->_delta;
                    //}
                    pNeuron->_delta *= pNeuron->ActivationFunctionPrime(pNeuron->_z);
                    pNeuron->_deltaUpdate += pNeuron->_delta;

                    for(SynapsePtr& pSynapse : pNeuron->_incomingEdges)
                    {
                        pSynapse->_pSource->_delta += pSynapse->_weight * pNeuron->_delta;
                        pSynapse->_wUpdate += pSynapse->_pSource->_a * pNeuron->_delta;
                    }
                    pNeuron->_delta = 0.0;
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

        std::vector<std::thread> threads;
        threads.reserve(this->_numLayers);
        auto& layers = this->_layers;
        double& lRate = this->_learningRate;
        for(int i = this->_numLayers - 1; i >= 0; --i)
        {
            threads.push_back(std::thread([i, &layers, lRate, miniBatchSize]()
            {
                for(NeuronPtr& pNeuron : layers[i])
                {
                    for(SynapsePtr& pSynapse : pNeuron->_incomingEdges)
                    {
                        pSynapse->Update(lRate / miniBatchSize);
                    }
                    pNeuron->Update(lRate / miniBatchSize);
                }
            }));
            //for(INeuronPtr& pNeuron : this->_layers[i])
            //{
            //    for(SynapsePtr& pSynapse : pNeuron->_incomingEdges)
            //    {
            //        pSynapse->Update(this->_learningRate / miniBatchSize);
            //    }
            //    pNeuron->Update(this->_learningRate / miniBatchSize);
            //}
        }
        for(std::thread& th : threads)
        {
            th.join();
        }
    }

} // end of namespace Neural
} // end of namespace Classifiers
