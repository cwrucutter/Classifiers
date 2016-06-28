// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "unitTest/Common/TestFunctions.hpp"
#include "unitTest/TestFixtures/NetTestFixtures.hpp"


namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    NetTestCppNeuron::NetTestCppNeuron()
    {
    }

    NetTestCppNeuron::~NetTestCppNeuron()
    {
    }

    void NetTestCppNeuron::SetNeuronParams(INeuronPtr pNeuron,
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

    bool NetTestCppNeuron::CheckSynapse(INeuronPtr getSynapse, int index,
        bool incoming, INeuronPtr pSource, INeuronPtr pDest)
    {
        if(incoming)
        {
            return getSynapse->_incomingEdges[index]->_pSource == pSource &&
                getSynapse->_incomingEdges[index]->_pDest == pDest;
        }
        return getSynapse->_outgoingEdges[index]->_pSource == pSource &&
            getSynapse->_outgoingEdges[index]->_pDest == pDest;
    }

    int NetTestCppNeuron::GetLengthOfEdges(INeuronPtr pNeuron, bool incoming)
    {
        if(incoming)
        {
            return pNeuron->_incomingEdges.size();
        }
        return pNeuron->_outgoingEdges.size();
    }

    void FFNeuralNetTest::SetUp()
    {
        this->_pNet =
            std::make_shared<FFNeuralNet>(std::vector<int>{2, 3, 1});
        this->_pNeuronChanger = std::make_shared<NetTestCppNeuron>();
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
                                pNet->_layers[i][j],
                                false
                            ),
                            pNet->_layers[i + 1].size()
                        );
                        EXPECT_EQ(
                            this->_pNeuronChanger->GetLengthOfEdges(
                                pNet->_layers[i + 1][k],
                                true
                            ),
                            pNet->_layers[i].size()
                        );
                        EXPECT_TRUE(this->_pNeuronChanger->CheckSynapse(
                            pNet->_layers[i][j],
                            k,
                            false,
                            pNet->_layers[i][j],
                            pNet->_layers[i + 1][k]
                        ));
                        EXPECT_TRUE(this->_pNeuronChanger->CheckSynapse(
                            pNet->_layers[i + 1][k],
                            j,
                            true,
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
        for(std::vector<INeuronPtr> layer : pNet->_layers)
        {
            for(INeuronPtr pNeuron : layer)
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

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
