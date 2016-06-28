// SYSTEM INCLUDES
#include <vector>

// C++ PROJECT INCLUDES
#include "Neural/Synapse.hpp"
#include "unitTest/Common/TestFunctions.hpp"
#include "unitTest/TestFixtures/NeuronTestFixtures.hpp"


namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    void CppNeuronTest::SetUp()
    {
        this->_pNeuron =
            std::make_shared<CppNeuron>(&Sigmoid);
    }

    void CppNeuronTest::TearDown()
    {
    }

    void CppNeuronTest::TestConstructor()
    {
        EXPECT_EQ(0.0, this->_pNeuron->_a) << "_a is initalized to 0";
        EXPECT_EQ(0.0, this->_pNeuron->_z) << "_z is initalized to 0";
        EXPECT_EQ(0.0, this->_pNeuron->_delta) << "_delta is initalized to 0";
        EXPECT_EQ(0.0, this->_pNeuron->_deltaUpdate) << "_deltaUpdate is initalized to 0";
        EXPECT_EQ(&Sigmoid, this->_pNeuron->_pActivationFunction) << "func ptr should be set";
        EXPECT_EQ(0, this->_pNeuron->_incomingEdges.size()) << "no synapses";
        EXPECT_EQ(0, this->_pNeuron->_outgoingEdges.size()) << "no synapses";
    }

    void CppNeuronTest::TestComputeValue()
    {
        // no syapses
        std::vector<CppNeuronPtr> pNeurons;
        this->_pNeuron->ComputeValue();
        EXPECT_EQ(Sigmoid(this->_pNeuron->_bias), this->_pNeuron->_a);

        // 1 synapse
        pNeurons.push_back(std::make_shared<CppNeuron>(&Sigmoid));
        this->_pNeuron->_incomingEdges.push_back(
            std::make_shared<Synapse>(pNeurons[pNeurons.size() - 1], this->_pNeuron)
        );
        this->_pNeuron->_incomingEdges[this->_pNeuron->_incomingEdges.size() - 1]\
            ->_weight = 1.0;
        pNeurons[pNeurons.size() - 1]->_a = 1.0;
        this->_pNeuron->ComputeValue();
        EXPECT_EQ(Sigmoid(this->_pNeuron->_bias + pNeurons.size()), this->_pNeuron->_a);

        // 2 synapses
        pNeurons.push_back(std::make_shared<CppNeuron>(&Sigmoid));
        this->_pNeuron->_incomingEdges.push_back(
            std::make_shared<Synapse>(pNeurons[pNeurons.size() - 1], this->_pNeuron)
        );
        this->_pNeuron->_incomingEdges[this->_pNeuron->_incomingEdges.size() - 1]\
            ->_weight = 1.0;
        pNeurons[pNeurons.size() - 1]->_a = 1.0;
        this->_pNeuron->ComputeValue();
        EXPECT_EQ(Sigmoid(this->_pNeuron->_bias + pNeurons.size()), this->_pNeuron->_a);


        // 3 synapses
        pNeurons.push_back(std::make_shared<CppNeuron>(&Sigmoid));
        this->_pNeuron->_incomingEdges.push_back(
            std::make_shared<Synapse>(pNeurons[pNeurons.size() - 1], this->_pNeuron)
        );
        this->_pNeuron->_incomingEdges[this->_pNeuron->_incomingEdges.size() - 1]\
            ->_weight = 1.0;
        pNeurons[pNeurons.size() - 1]->_a = 1.0;
        this->_pNeuron->ComputeValue();
        EXPECT_EQ(Sigmoid(this->_pNeuron->_bias + pNeurons.size()), this->_pNeuron->_a);

        // many (10) synapses
        for(int i = 0; i < 7; ++i)
        {
            pNeurons.push_back(std::make_shared<CppNeuron>(&Sigmoid));
            this->_pNeuron->_incomingEdges.push_back(
                std::make_shared<Synapse>(pNeurons[pNeurons.size() - 1], this->_pNeuron)
            );
            this->_pNeuron->_incomingEdges[this->_pNeuron->_incomingEdges.size() - 1]\
                ->_weight = 1.0;
            pNeurons[pNeurons.size() - 1]->_a = 1.0;
            this->_pNeuron->ComputeValue();
            EXPECT_EQ(Sigmoid(this->_pNeuron->_bias + pNeurons.size()), this->_pNeuron->_a);
        }
    }

    void CppNeuronTest::TestUpdate()
    {
        this->_pNeuron->_bias = 1.0;
        this->_pNeuron->Update(2.0);
        EXPECT_EQ(1.0, this->_pNeuron->_bias);
        EXPECT_EQ(0.0, this->_pNeuron->_deltaUpdate);

        this->_pNeuron->_deltaUpdate = 5.0;
        this->_pNeuron->Update(2.0);
        EXPECT_EQ(-9.0, this->_pNeuron->_bias);
        EXPECT_EQ(0.0, this->_pNeuron->_deltaUpdate);
    }

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
