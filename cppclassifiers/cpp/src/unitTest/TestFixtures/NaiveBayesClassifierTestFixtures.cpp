// SYSTEM INCLUDES
#include <algorithm>
#include <map>
#include <vector>


// C++ PROJECT INCLUDES
#include "unitTest/TestFixtures/NaiveBayesClassifierTestFixtures.hpp"
#include "unitTest/Common/TestStateFunctors.hpp"

namespace Classifiers
{
namespace Bayesian
{
namespace Tests
{

    void NaiveBayesClassifierTest::SetUp()
    {
    }

    void NaiveBayesClassifierTest::TearDown()
    {
    }

    void NaiveBayesClassifierTest::TestNaiveBayesConstructor()
    {
        std::map<std::string, StateFunctorPtr> varMap = {
            {"has-fur?", MakeBooleanFunctor()},
            {"long-teeth?", MakeBooleanFunctor()},
            {"scary?", MakeBooleanFunctor()}
        };
        NaiveBayesClassifierPtr pClassifier =
            std::make_shared<NaiveBayesClassifier>(
                "lion", MakeBooleanFunctor(),
                varMap
            );
        EXPECT_EQ("lion", pClassifier->_varName);
        EXPECT_EQ(2, pClassifier->_varNameFunctor->NumStates());
        EXPECT_EQ(4, pClassifier->_nodes.size());
        for(auto& name : std::vector<std::string>{"has-fur?", "long-teeth?", "scary?"})
        {
            EXPECT_NE(std::find(pClassifier->_featureNames.begin(),
                                pClassifier->_featureNames.end(),
                                name),
                      pClassifier->_featureNames.end());
        }
    }

    void NaiveBayesClassifierTest::TestNaiveBayesTrain()
    {
        std::map<std::string, StateFunctorPtr> varMap = {
            {"has-fur?", MakeBooleanFunctor()},
            {"long-teeth?", MakeBooleanFunctor()},
            {"scary?", MakeBooleanFunctor()}
        };
        NaiveBayesClassifierPtr pClassifier =
            std::make_shared<NaiveBayesClassifier>(
                "lion", MakeBooleanFunctor(),
                varMap
            );

        std::vector<std::map<std::string, std::string> > sampleTrainingData = {
            {
                {"has-fur?", "true"},
                {"long-teeth?", "false"},
                {"scary?", "false"},
                {"lion", "false"}
            },
            {
                {"has-fur?", "false"},
                {"long-teeth?", "true"},
                {"scary?", "true"},
                {"lion", "false"}
            },
            {
                {"has-fur?", "true"},
                {"long-teeth?", "true"},
                {"scary?", "true"},
                {"lion", "true"}
            }
        };

        pClassifier->Train(sampleTrainingData);

        BayesianNodePtr lionNode = pClassifier->_nodes["lion"];
        BayesianNodePtr hasFurNode = pClassifier->_nodes["has-fur?"];
        BayesianNodePtr longTeethNode = pClassifier->_nodes["long-teeth?"];
        BayesianNodePtr scaryNode = pClassifier->_nodes["scary?"];

        std::vector<std::vector<double> > expectedConditionalsTable = {
            {0.5,   0.5},
            {0.0,   1.0}
        };
        std::vector<double> expectedLionVector = {0.67, 0.33};
        for(int i = 0; i < expectedLionVector.size(); ++i)
        {
            EXPECT_NEAR(expectedLionVector[i], lionNode->_table[0][i],
                0.009999999999999);
        }
        for(int i = 0; i < expectedConditionalsTable.size(); ++i)
        {
            for(int j = 0; j < expectedConditionalsTable[i].size(); ++j)
            {
                EXPECT_NEAR(expectedConditionalsTable[i][j],
                    hasFurNode->_table[i][j], 0.0999999999999999);
            }
        }
        for(int i = 0; i < expectedConditionalsTable.size(); ++i)
        {
            for(int j = 0; j < expectedConditionalsTable[i].size(); ++j)
            {
                EXPECT_NEAR(expectedConditionalsTable[i][j],
                    longTeethNode->_table[i][j], 0.0999999999999999);
            }
        }
        for(int i = 0; i < expectedConditionalsTable.size(); ++i)
        {
            for(int j = 0; j < expectedConditionalsTable[i].size(); ++j)
            {
                EXPECT_NEAR(expectedConditionalsTable[i][j],
                    scaryNode->_table[i][j], 0.0999999999999999);
            }
        }

    }

    void NaiveBayesClassifierTest::TestNaiveBayesPrOfState()
    {
        std::map<std::string, StateFunctorPtr> varMap = {
            {"has-fur?", MakeBooleanFunctor()},
            {"long-teeth?", MakeBooleanFunctor()},
            {"scary?", MakeBooleanFunctor()}
        };
        NaiveBayesClassifierPtr pClassifier =
            std::make_shared<NaiveBayesClassifier>(
                "lion", MakeBooleanFunctor(),
                varMap
            );
        pClassifier->_nodes["lion"]->_table = {
            {0.9,   0.1}
        };
        pClassifier->_nodes["has-fur?"]->_table = {
            {0.9,   0.1},
            {0.5,   0.5}
        };
        pClassifier->_nodes["long-teeth?"]->_table = {
            {0.5,   0.5},
            {0.1,   0.9}
        };
        pClassifier->_nodes["scary?"]->_table = {
            {0.5,   0.5},
            {0.2,   0.8}
        };

        std::map<std::string, std::string> featureVector = {
            {"has-fur?", "true"},
            {"long-teeth?", "false"},
            {"scary?", "false"}
        };

        std::vector<std::string> lionStates = {"false", "true"};
        EXPECT_NEAR(0.001, pClassifier->PrOfState(featureVector, lionStates[1]),
            0.000999999999999999);
        EXPECT_NEAR(0.0225, pClassifier->PrOfState(featureVector, lionStates[0]),
            0.0000999999999999999);
    }

    void NaiveBayesClassifierTest::TestNaiveBayesClassify()
    {
        std::map<std::string, StateFunctorPtr> varMap = {
            {"has-fur?", MakeBooleanFunctor()},
            {"long-teeth?", MakeBooleanFunctor()},
            {"scary?", MakeBooleanFunctor()}
        };
        NaiveBayesClassifierPtr pClassifier =
            std::make_shared<NaiveBayesClassifier>(
                "lion", MakeBooleanFunctor(),
                varMap
            );
        pClassifier->_nodes["lion"]->_table = {
            {0.9,   0.1}
        };
        pClassifier->_nodes["has-fur?"]->_table = {
            {0.9,   0.1},
            {0.5,   0.5}
        };
        pClassifier->_nodes["long-teeth?"]->_table = {
            {0.5,   0.5},
            {0.1,   0.9}
        };
        pClassifier->_nodes["scary?"]->_table = {
            {0.5,   0.5},
            {0.2,   0.8}
        };

        std::map<std::string, std::string> featureVector = {
            {"has-fur?", "true"},
            {"long-teeth?", "false"},
            {"scary?", "false"}
        };
        EXPECT_EQ(std::vector<std::string>{"false"},
                pClassifier->Classify(featureVector));

    }

} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers
