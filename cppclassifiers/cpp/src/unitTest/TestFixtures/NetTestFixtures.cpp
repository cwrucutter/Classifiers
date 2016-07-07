// SYSTEM INCLUDES
#include <random>
#include <list>

// C++ PROJECT INCLUDES
#include "unitTest/Common/TestFunctions.hpp"
#include "unitTest/TestFixtures/NetTestFixtures.hpp"


namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    NetTestNeuron::NetTestNeuron()
    {
    }

    NetTestNeuron::~NetTestNeuron()
    {
    }

    void NetTestNeuron::SetNeuronParams(NeuronPtr pNeuron,
                                           std::map<std::string, double> dict)
    {
        pNeuron->_a = dict["a"];
        pNeuron->_z = dict["z"];
        pNeuron->_delta = dict["delta"];
        pNeuron->_deltaUpdate = dict["deltaUpdate"];
        pNeuron->_bias = dict["bias"];
        for(SynapsePtr pSynapse : pNeuron->_incomingEdges)
        {
            pSynapse->_weight = dict["weight"];
        }
    }

    bool NetTestNeuron::CheckSynapse(NeuronPtr getSynapse, int index,
        NeuronPtr pSource, NeuronPtr pDest)
    {
        return getSynapse->_incomingEdges[index]->_pSource == pSource &&
            getSynapse->_incomingEdges[index]->_pDest == pDest;
    }

    int NetTestNeuron::GetLengthOfEdges(NeuronPtr pNeuron)
    {
        return pNeuron->_incomingEdges.size();
    }

    double NetTestNeuron::GetBias(NeuronPtr pNeuron)
    {
        return pNeuron->_bias;
    }

    double NetTestNeuron::GetDeltaUpdate(NeuronPtr pNeuron)
    {
        return pNeuron->_deltaUpdate;
    }

    std::vector<SynapsePtr> NetTestNeuron::GetEdges(NeuronPtr pNeuron)
    {
        return pNeuron->_incomingEdges;
    }





    void FFNeuralNetTest::SetUp()
    {
        this->_pNet =
            std::make_shared<FFNeuralNet>(std::vector<int>{2, 3, 1});
        this->_pNeuronChanger = std::make_shared<NetTestNeuron>();
    }

    void FFNeuralNetTest::TearDown()
    {
    }

    void FFNeuralNetTest::CheckGraphStructure(FFNeuralNetPtr pNet, std::vector<int> layerScheme)
    {
        EXPECT_EQ(layerScheme.size(), pNet->_numLayers);
        for(int i = 0; i < pNet->_numLayers; ++i)
        {
            EXPECT_EQ(layerScheme[i], pNet->_layers[i].size());

            if(i < pNet->_numLayers - 1)
            {
                for(int j = 0; j < pNet->_layers[i].size(); ++j)
                {
                    for(int k = 0; k < pNet->_layers[i + 1].size(); ++k)
                    {
                        EXPECT_EQ(
                            this->_pNeuronChanger->GetLengthOfEdges(
                                pNet->_layers[i + 1][k]
                            ),
                            pNet->_layers[i].size()
                        );
                        EXPECT_TRUE(this->_pNeuronChanger->CheckSynapse(
                            pNet->_layers[i + 1][k],
                            j,
                            pNet->_layers[i][j],
                            pNet->_layers[i + 1][k]
                        ));
                    }
                }
            }
        }
    }

    void FFNeuralNetTest::SetupGraph(FFNeuralNetPtr pNet, std::map<std::string, double> params)
    {
        for(std::vector<NeuronPtr> layer : pNet->_layers)
        {
            for(NeuronPtr pNeuron : layer)
            {
                this->_pNeuronChanger->SetNeuronParams(pNeuron, params);
            }
        }
    }

    void FFNeuralNetTest::TestFFNetConstructor()
    {
        std::vector<int> layerScheme = {2, 3, 1};
        this->CheckGraphStructure(this->_pNet, layerScheme);

        layerScheme = {784, 30, 10};
        this->CheckGraphStructure(std::make_shared<FFNeuralNet>(layerScheme), layerScheme);
    }

    void FFNeuralNetTest::TestFFNetFeedForward()
    {
        std::map<std::string, double> params = {
            {"a", 0.0},
            {"z", 0.0},
            {"delta", 0.0},
            {"deltaUpdate", 0.0},
            {"bias", 1.0},
            {"weight", 1.0}
        };
        this->SetupGraph(this->_pNet, params);

        // every edge should be weighted 1.0, every bias should be 1.0.
        std::vector<double> inputs = {1.0, 1.0};

        double output = Sigmoid(1.0 + 3 * Sigmoid(3.0));

        std::vector<double> expectedOutputs = {output};
        EXPECT_EQ(expectedOutputs, this->_pNet->FeedForward(inputs));

        this->_pNet = std::make_shared<FFNeuralNet>(std::vector<int>{2, 3, 2});
        this->SetupGraph(this->_pNet, params);

        expectedOutputs = {output, output};
        EXPECT_EQ(expectedOutputs, this->_pNet->FeedForward(inputs));
    }

    void FFNeuralNetTest::TestFFNetBackPropogate()
    {
        this->_pNet = std::make_shared<FFNeuralNet>(std::vector<int>{2, 3, 1});
        std::map<std::string, double> params = {
            {"a", 0.0},
            {"z", 0.0},
            {"delta", 0.0},
            {"deltaUpdate", 0.0},
            {"bias", 1.0},
            {"weight", 1.0}
        };
        this->SetupGraph(this->_pNet, params);

        std::vector<std::pair<std::vector<double>,
                              std::vector<double> > > evalData =
        {
            {{1.0, 1.0}, {0.5}}
        };
        this->_pNet->BackPropogate(std::get<0>(evalData[0]), std::get<1>(evalData[0]));

        // check _wUpdates and _deltaUpdates for each node and synapse
        std::vector<NeuronPtr> inputLayer = this->_pNet->_layers[0];
        std::vector<NeuronPtr> hiddenLayer = this->_pNet->_layers[1];
        std::vector<NeuronPtr> outputLayer = this->_pNet->_layers[2];

        for(NeuronPtr pNeuron : inputLayer)
        {
            EXPECT_NEAR(0.0, this->_pNeuronChanger->GetDeltaUpdate(pNeuron),
                0.00000001);
        }
        for(NeuronPtr pNeuron : hiddenLayer)
        {
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(0.00169483013962, pSynapse->_wUpdate,
                    0.000000001);
            }
            EXPECT_NEAR(0.00169483013962, this->_pNeuronChanger->GetDeltaUpdate(pNeuron),
                0.000000001);
        }
        for(NeuronPtr pNeuron : outputLayer)
        {
            EXPECT_NEAR(0.0192134135835, this->_pNeuronChanger->GetDeltaUpdate(pNeuron),
                0.00000001);
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(0.0183022006676, pSynapse->_wUpdate,
                    0.00000001);
            }
        }
    }

    void FFNeuralNetTest::TestFFNetMiniBatchUpdate()
    {
        this->_pNet = std::make_shared<FFNeuralNet>(std::vector<int>{2, 3, 1});
        std::map<std::string, double> params = {
            {"a", 0.0},
            {"z", 0.0},
            {"delta", 0.0},
            {"deltaUpdate", 0.0},
            {"bias", 1.0},
            {"weight", 1.0}
        };
        this->SetupGraph(this->_pNet, params);

        std::vector<std::pair<std::vector<double>,
                              std::vector<double> > > evalData =
        {
            {{1.0, 1.0}, {0.5}}
        };
        this->_pNet->MiniBatchUpdate(evalData, 0, 1);

        // check _wUpdates and _deltaUpdates for each node and synapse
        std::vector<NeuronPtr> inputLayer = this->_pNet->_layers[0];
        std::vector<NeuronPtr> hiddenLayer = this->_pNet->_layers[1];
        std::vector<NeuronPtr> outputLayer = this->_pNet->_layers[2];

        for(NeuronPtr pNeuron : inputLayer)
        {
            EXPECT_NEAR(1.0 - 0.0, this->_pNeuronChanger->GetBias(pNeuron),
                0.00000001);
        }
        for(NeuronPtr pNeuron : hiddenLayer)
        {
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(1.0 - 0.00169483013962, pSynapse->_weight,
                    0.000000001);
            }
            EXPECT_NEAR(1.0 - 0.00169483013962, this->_pNeuronChanger->GetBias(pNeuron),
                0.000000001);
        }
        for(NeuronPtr pNeuron : outputLayer)
        {
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(1.0 - 0.0183022006676, pSynapse->_weight,
                    0.00000001);
            }
            EXPECT_NEAR(1.0 - 0.0192134135835, this->_pNeuronChanger->GetBias(pNeuron),
                0.00000001);
        }

        this->_pNet->MiniBatchUpdate(evalData, 0, 1);

        inputLayer = this->_pNet->_layers[0];
        hiddenLayer = this->_pNet->_layers[1];
        outputLayer = this->_pNet->_layers[2];

        for(NeuronPtr pNeuron : inputLayer)
        {
            EXPECT_NEAR(1.0, this->_pNeuronChanger->GetBias(pNeuron),
                0.00000001);
        }
        for(NeuronPtr pNeuron : hiddenLayer)
        {
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(0.996521334375, pSynapse->_weight,
                    0.000000001);
            }
            EXPECT_NEAR(0.996521334375, this->_pNeuronChanger->GetBias(pNeuron),
                0.000000001);
        }
        for(NeuronPtr pNeuron : outputLayer)
        {
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(0.962167804037, pSynapse->_weight,
                    0.00000001);
            }
            EXPECT_NEAR(0.960279293461, this->_pNeuronChanger->GetBias(pNeuron),
                0.00000001);
        }
    }

    void FFNeuralNetTest::TestFFNetMiniBatchPartitionAlgorithm()
    {
        using InputData_t = std::vector<std::pair<std::vector<double>,
                                                  std::vector<double> > >;
        using MiniBatch_t = std::vector<std::pair<std::vector<double>,
                                                std::vector<double> > >;

        std::vector<MiniBatch_t> testMiniBatches;
        InputData_t inputData = {
            {{1.0, 2.0}, {10.0}},
            {{3.0, 4.0}, {20.0}},
            {{5.0, 6.0}, {30.0}},
            {{7.0, 8.0}, {40.0}},
            {{9.0, 10.0}, {50.0}},
            {{11.0, 12.0}, {60.0}},
            {{13.0, 14.0}, {70.0}},
            {{15.0, 16.0}, {80.0}},
            {{17.0, 18.0}, {90.0}},
            {{19.0, 20.0}, {100.0}}
        };
        int n = (int)inputData.size();
        int miniBatchSize = 2;

        for(int k = 0; k < n; k += miniBatchSize)
        {
            MiniBatch_t miniBatch;
            for(int l = k; l < k + miniBatchSize; ++l)
            {
                miniBatch.push_back(inputData[l]);
            }
            testMiniBatches.push_back(miniBatch);
        }

        std::vector<MiniBatch_t> expectedMiniBatches = {
            {
                {{1.0, 2.0}, {10.0}},
                {{3.0, 4.0}, {20.0}}
            },
            {
                {{5.0, 6.0}, {30.0}},
                {{7.0, 8.0}, {40.0}}
            },
            {
                {{9.0, 10.0}, {50.0}},
                {{11.0, 12.0}, {60.0}}
            },
            {
                {{13.0, 14.0}, {70.0}},
                {{15.0, 16.0}, {80.0}}
            },
            {
                {{17.0, 18.0}, {90.0}},
                {{19.0, 20.0}, {100.0}}
            }
        };

        EXPECT_EQ(expectedMiniBatches, testMiniBatches);
    }

    void FFNeuralNetTest::TestFFNetTrain()
    {
        this->_pNet = std::make_shared<FFNeuralNet>(std::vector<int>{2, 3, 1});
        std::map<std::string, double> params = {
            {"a", 0.0},
            {"z", 0.0},
            {"delta", 0.0},
            {"deltaUpdate", 0.0},
            {"bias", 1.0},
            {"weight", 1.0}
        };
        this->SetupGraph(this->_pNet, params);

        std::vector<std::pair<std::vector<double>,
                              std::vector<double> > > evalData =
        {
            {{1.0, 1.0}, {0.5}}
        };

        this->_pNet->Train(evalData, 1, 1);

        // check _wUpdates and _deltaUpdates for each node and synapse
        std::vector<NeuronPtr> inputLayer = this->_pNet->_layers[0];
        std::vector<NeuronPtr> hiddenLayer = this->_pNet->_layers[1];
        std::vector<NeuronPtr> outputLayer = this->_pNet->_layers[2];

        for(NeuronPtr pNeuron : inputLayer)
        {
            EXPECT_NEAR(1.0, this->_pNeuronChanger->GetBias(pNeuron),
                0.00000001);
        }
        for(NeuronPtr pNeuron : hiddenLayer)
        {
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(1.0 - 0.00169483013962, pSynapse->_weight,
                    0.000000001);
            }
            EXPECT_NEAR(1.0 - 0.00169483013962, this->_pNeuronChanger->GetBias(pNeuron),
                0.000000001);
        }
        for(NeuronPtr pNeuron : outputLayer)
        {
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(1.0 - 0.0183022006676, pSynapse->_weight,
                    0.00000001);
            }
            EXPECT_NEAR(1.0 - 0.0192134135835, this->_pNeuronChanger->GetBias(pNeuron),
                0.00000001);
        }

        this->_pNet = std::make_shared<FFNeuralNet>(std::vector<int>{2, 3, 1});
        this->SetupGraph(this->_pNet, params);
        this->_pNet->Train(evalData, 2, 1);

        inputLayer = this->_pNet->_layers[0];
        hiddenLayer = this->_pNet->_layers[1];
        outputLayer = this->_pNet->_layers[2];

        for(NeuronPtr pNeuron : inputLayer)
        {
            EXPECT_NEAR(1.0, this->_pNeuronChanger->GetBias(pNeuron),
                0.00000001);
        }
        for(NeuronPtr pNeuron : hiddenLayer)
        {
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(0.996521334375, pSynapse->_weight,
                    0.000000001);
            }
            EXPECT_NEAR(0.996521334375, this->_pNeuronChanger->GetBias(pNeuron),
                0.000000001);
        }
        for(NeuronPtr pNeuron : outputLayer)
        {
            for(SynapsePtr pSynapse : this->_pNeuronChanger->GetEdges(pNeuron))
            {
                EXPECT_NEAR(0.962167804037, pSynapse->_weight,
                    0.00000001);
            }
            EXPECT_NEAR(0.960279293461, this->_pNeuronChanger->GetBias(pNeuron),
                0.00000001);
        }

    }

    void FFNeuralNetTest::TestFFNetDefaultEvaluationFunction()
    {
        this->_pNet = std::make_shared<FFNeuralNet>(std::vector<int>{2, 3, 1});
        std::map<std::string, double> params = {
            {"a", 0.0},
            {"z", 0.0},
            {"delta", 0.0},
            {"deltaUpdate", 0.0},
            {"bias", 1.0},
            {"weight", 1.0}
        };
        this->SetupGraph(this->_pNet, params);

        std::vector<std::pair<std::vector<double>,
                              std::vector<double> > > evalData =
        {
            {{1.0, 1.0}, {0}}
        };

        EXPECT_EQ(1, DefaultEvalFunc(evalData, this->_pNet.get()));
    }




    /**
    void FFNeuralNetSpeedTest::SetUp()
    {
        std::vector<int> layersScheme = {784, 30, 10};
        this->_pNet = std::make_shared<FFNeuralNet>(layersScheme);

        // generate 50,000 784 dimensional vectors of grayscaled values
        this->_tData;
        this->_testData;
        std::default_random_engine generator;
        std::uniform_real_distribution<double> unif(0.0, 1.0);
        for(int i = 0; i < 30000; ++i)
        {
            std::vector<double> input;
            std::vector<double> output;

            for(int j = 0; j < 784; ++j)
            {
                input.push_back(unif(generator));
            }
            for(int j = 0; j < 10; ++j)
            {
                output.push_back((double)(int)(unif(generator) * 10));
            }
            this->_tData.push_back(std::make_pair(input, output));
        }
        for(int i = 0; i < 10000; ++i)
        {
            std::vector<double> input;
            std::vector<double> output;

            for(int j = 0; j < 784; ++j)
            {
                input.push_back(unif(generator));
            }
            output.push_back((double)(int)(unif(generator) * 9));
            this->_testData.push_back(std::make_pair(input, output));
        }
    }

    void FFNeuralNetSpeedTest::TearDown()
    {
    }

    void FFNeuralNetSpeedTest::FFNetFeedForwardSpeedTest()
    {
        std::vector<int> layersScheme = {784, 30, 10};
        std::default_random_engine generator;
        std::normal_distribution<double> dist(0.0, 1.0);
        std::vector<double> inputs;

        for(int i = 0; i < layersScheme[0]; ++i)
        {
            inputs.push_back(dist(generator));
        }
        this->_pNet->FeedForward(inputs);
    }

    void FFNeuralNetSpeedTest::FFNetTrainSpeedTest()
    {
        this->_pNet->Train(this->_tData, 1, 10, this->_testData);
    }
    */

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
