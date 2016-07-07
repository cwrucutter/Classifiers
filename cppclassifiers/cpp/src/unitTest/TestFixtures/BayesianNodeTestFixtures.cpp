// SYSTEM INCLUDES
#include <map>
#include <vector>

// C++ PROJECT INCLUDES
#include "unitTest/TestFixtures/BayesianNodeTestFixtures.hpp"
#include "unitTest/Common/TestStateFunctors.hpp"

namespace Classifiers
{
namespace Bayesian
{
namespace Tests
{

    void BayesianNodeTest::SetUp()
    {
    }

    void BayesianNodeTest::TearDown()
    {
    }

    void BayesianNodeTest::TestNodeConstructor()
    {
        BayesianNodePtr pNode = std::make_shared<BayesianNode>("test",
            MakeBooleanFunctor(), DependencyType{});

        std::vector<std::vector<double> > expectedTable;
        std::vector<double> expectedTotalProbabilities;
        std::vector<std::string> expectedDepNames;
        std::vector<std::string> actualDepNames;

        expectedTable = {{0.0, 0.0}};
        expectedTotalProbabilities = {};
        expectedDepNames = {};

        EXPECT_EQ(1, pNode->_numRows);
        EXPECT_EQ(expectedTable, pNode->_table);
        EXPECT_EQ("test", pNode->_name);
        EXPECT_EQ(MakeBooleanFunctor()->GetAllStates(),
                  pNode->_nameStateFunctor->GetAllStates());
        EXPECT_EQ(expectedDepNames.size(), pNode->_dependencies.size());
        EXPECT_EQ(0, pNode->_parents.size());
        EXPECT_EQ(expectedTotalProbabilities, pNode->_totalProbabilities);

        pNode = std::make_shared<BayesianNode>("test", MakeEyeColorFunctor(),
            DependencyType{});

        expectedTable = {{0.0, 0.0, 0.0, 0.0}};
        expectedTotalProbabilities = {};
        expectedDepNames = {};

        EXPECT_EQ(1, pNode->_numRows);
        EXPECT_EQ(expectedTable, pNode->_table);
        EXPECT_EQ("test", pNode->_name);
        EXPECT_EQ(MakeEyeColorFunctor()->GetAllStates(),
                  pNode->_nameStateFunctor->GetAllStates());
        EXPECT_EQ(expectedDepNames.size(), pNode->_dependencies.size());
        EXPECT_EQ(0, pNode->_parents.size());
        EXPECT_EQ(expectedTotalProbabilities, pNode->_totalProbabilities);

        pNode = std::make_shared<BayesianNode>("test", MakeBooleanFunctor(),
            DependencyType{{"a", MakeEyeColorFunctor()}});

        expectedTable = {
            {0.0, 0.0},
            {0.0, 0.0},
            {0.0, 0.0},
            {0.0, 0.0}
        };
        expectedTotalProbabilities = {0.0, 0.0};
        expectedDepNames = {"a"};
        actualDepNames.clear();

        EXPECT_EQ(4, pNode->_numRows);
        EXPECT_EQ(expectedTable, pNode->_table);
        EXPECT_EQ("test", pNode->_name);
        EXPECT_EQ(MakeBooleanFunctor()->GetAllStates(),
                  pNode->_nameStateFunctor->GetAllStates());
        EXPECT_EQ(expectedDepNames.size(), pNode->_dependencies.size());
        for(auto& dep : pNode->_dependencies)
        {
            actualDepNames.push_back(std::get<0>(dep));
        }
        EXPECT_EQ(expectedDepNames, actualDepNames);
        EXPECT_EQ(1, pNode->_parents.size());
        EXPECT_EQ(expectedTotalProbabilities, pNode->_totalProbabilities);


        pNode = std::make_shared<BayesianNode>("test", MakeBooleanFunctor(),
            DependencyType{
                {"a", MakeHairColorFunctor()},
                {"b", MakeEyeColorFunctor()}
            });
            
        expectedTable.clear();
        expectedTable.reserve(12);
        for(int i = 0; i < 12; ++i)
        {
            expectedTable.push_back(std::vector<double>{0.0, 0.0});
        }
        expectedTotalProbabilities = {0.0, 0.0};
        expectedDepNames = {
            "b",
            "a"
        };
        actualDepNames.clear();

        EXPECT_EQ(12, pNode->_numRows);
        EXPECT_EQ(expectedTable, pNode->_table);
        EXPECT_EQ("test", pNode->_name);
        EXPECT_EQ(MakeBooleanFunctor()->GetAllStates(),
                  pNode->_nameStateFunctor->GetAllStates());
        EXPECT_EQ(expectedDepNames.size(), pNode->_dependencies.size());
        for(auto& dep : pNode->_dependencies)
        {
            actualDepNames.push_back(std::get<0>(dep));
        }
        EXPECT_EQ(expectedDepNames, actualDepNames);
        EXPECT_EQ(2, pNode->_parents.size());
        EXPECT_EQ(expectedTotalProbabilities, pNode->_totalProbabilities);

        pNode = std::make_shared<BayesianNode>("test", MakeBooleanFunctor(),
            DependencyType{
                {"a", MakeBooleanFunctor()},    // (a, 2)
                {"b", MakeBooleanFunctor()},    // (b, 2)
                {"e", MakeEyeColorFunctor()},   // (e, 4)
                {"d", MakeHairColorFunctor()},  // (d, 3)
                {"c", MakeEyeColorFunctor()}    // (c, 4)
            });

        /**
            first sort...by letter in increasing order:

                (a, 2)
                (b, 2)
                (c, 4)
                (d, 3)
                (e, 4)

            second sort...by number (descending)

                (c, 4)
                (e, 4)
                (d, 3)
                (a, 2)
                (b, 2)

        */

        expectedTable.clear();
        expectedTable.reserve(192);
        for(int i = 0; i < 192; ++i)
        {
            expectedTable.push_back(std::vector<double>{0.0, 0.0});
        }
        expectedTotalProbabilities = {0.0, 0.0};
        expectedDepNames = {
            "c",
            "e",
            "d",
            "a",
            "b"
        };
        actualDepNames.clear();

        EXPECT_EQ(192, pNode->_numRows);
        EXPECT_EQ(expectedTable, pNode->_table);
        EXPECT_EQ("test", pNode->_name);
        EXPECT_EQ(MakeBooleanFunctor()->GetAllStates(),
                  pNode->_nameStateFunctor->GetAllStates());
        EXPECT_EQ(expectedDepNames.size(), pNode->_dependencies.size());
        for(auto& dep : pNode->_dependencies)
        {
            actualDepNames.push_back(std::get<0>(dep));
        }
        EXPECT_EQ(expectedDepNames, actualDepNames);
        EXPECT_EQ(5, pNode->_parents.size());
        EXPECT_EQ(expectedTotalProbabilities, pNode->_totalProbabilities);

    }

    void BayesianNodeTest::TestNodeValuesToIndex()
    {
        BayesianNodePtr pNode = std::make_shared<BayesianNode>("test", MakeBooleanFunctor(),
            DependencyType{});
        std::map<std::string, std::string> depValues = {};
        int expectedIndex = 0;
        StateFunctorPtr pFunctor = nullptr;
        StateFunctorPtr booleans = MakeBooleanFunctor();
        StateFunctorPtr hair = MakeHairColorFunctor();
        StateFunctorPtr eyes = MakeEyeColorFunctor();


        EXPECT_EQ(expectedIndex, pNode->ValuesToIndex(depValues));

        pNode = std::make_shared<BayesianNode>("test", MakeEyeColorFunctor(),
            DependencyType{});
        depValues = {};
        expectedIndex = 0;
        pFunctor = nullptr;

        EXPECT_EQ(expectedIndex, pNode->ValuesToIndex(depValues));

        pNode = std::make_shared<BayesianNode>("test", MakeBooleanFunctor(),
            DependencyType{
                {"a", MakeEyeColorFunctor()}
            });
        expectedIndex = 0;
        pFunctor = MakeEyeColorFunctor();
        depValues.clear();
        for(auto& state : pFunctor->GetAllStates())
        {
            depValues = {{"a", state}};
            EXPECT_EQ(expectedIndex, pNode->ValuesToIndex(depValues));
            ++expectedIndex;
        }

        pNode = std::make_shared<BayesianNode>("test", MakeEyeColorFunctor(),
            DependencyType{
                {"a", MakeBooleanFunctor()},
                {"b", MakeBooleanFunctor()}
            });
        expectedIndex = 0;
        pFunctor = MakeBooleanFunctor();
        depValues.clear();
        for(auto state1 : pFunctor->GetAllStates())
        {
            depValues["a"] = state1;
            for(auto state2 : pFunctor->GetAllStates())
            {
                depValues["b"] = state2;
                EXPECT_EQ(expectedIndex, pNode->ValuesToIndex(depValues));
                ++expectedIndex;
            }
        }

        pNode = std::make_shared<BayesianNode>("test", MakeEyeColorFunctor(),
            DependencyType{
                {"a", MakeEyeColorFunctor()},
                {"b", MakeHairColorFunctor()}
            });
        expectedIndex = 0;
        pFunctor = nullptr;
        depValues.clear();
        for(auto& state1 : eyes->GetAllStates())
        {
            depValues["a"] = state1;
            for(auto& state2 : hair->GetAllStates())
            {
                depValues["b"] = state2;
                EXPECT_EQ(expectedIndex, pNode->ValuesToIndex(depValues));
                ++expectedIndex;
            }
        }

        pNode = std::make_shared<BayesianNode>("test", MakeBooleanFunctor(),
            DependencyType{
                {"x", MakeHairColorFunctor()},
                {"y", MakeBooleanFunctor()},
                {"z", MakeBooleanFunctor()}
            });
        expectedIndex = 0;
        pFunctor = nullptr;
        depValues.clear();
        for(auto state1 : hair->GetAllStates())
        {
            depValues["x"] = state1;
            for(auto state2 : booleans->GetAllStates())
            {
                depValues["y"] = state2;
                for(auto state3 : booleans->GetAllStates())
                {
                    depValues["z"] = state3;
                    EXPECT_EQ(expectedIndex, pNode->ValuesToIndex(depValues));
                    ++expectedIndex;
                }
            }
        }

        pNode = std::make_shared<BayesianNode>("test", MakeBooleanFunctor(),
            DependencyType{
                {"x", MakeHairColorFunctor()},
                {"y", MakeBooleanFunctor()},
                {"z", MakeEyeColorFunctor()}
            });
        expectedIndex = 0;
        pFunctor = nullptr;
        depValues.clear();
        for(auto state1 : eyes->GetAllStates())
        {
            depValues["z"] = state1;
            for(auto state2 : hair->GetAllStates())
            {
                depValues["x"] = state2;
                for(auto state3 : booleans->GetAllStates())
                {
                    depValues["y"] = state3;
                    EXPECT_EQ(expectedIndex, pNode->ValuesToIndex(depValues));
                    ++expectedIndex;
                }
            }
        }

        pNode = std::make_shared<BayesianNode>("test", MakeBooleanFunctor(),
            DependencyType{
                {"a", MakeBooleanFunctor()},
                {"b", MakeBooleanFunctor()},
                {"e", MakeEyeColorFunctor()},
                {"d", MakeHairColorFunctor()},
                {"c", MakeEyeColorFunctor()}
            });
        expectedIndex = 0;
        pFunctor = nullptr;
        depValues.clear();
        for(auto state1 : eyes->GetAllStates())
        {
            depValues["c"] = state1;
            for(auto state2 : eyes->GetAllStates())
            {
                depValues["e"] = state2;
                for(auto state3 : hair->GetAllStates())
                {
                    depValues["d"] = state3;
                    for(auto state4 : booleans->GetAllStates())
                    {
                        depValues["a"] = state4;
                        for(auto state5 : booleans->GetAllStates())
                        {
                            depValues["b"] = state5;
                            EXPECT_EQ(expectedIndex, pNode->ValuesToIndex(depValues));
                            ++expectedIndex;
                        }
                    }
                }
            }
        }
        
    }

    void BayesianNodeTest::TestNodeAccessTable()
    {
        StateFunctorPtr booleans = MakeBooleanFunctor();
        StateFunctorPtr hair = MakeHairColorFunctor();
        StateFunctorPtr eyes = MakeEyeColorFunctor();
        std::map<std::string, std::string> depValues = {};
        int rowIndex = 0;
        int colIndex = 0;

        BayesianNodePtr pNode = std::make_shared<BayesianNode>("q", MakeBooleanFunctor(),
            DependencyType{});
        pNode->_table = {
            {0.7, 0.3}
        };
        
        for(int i = 0; i < pNode->_numRows; ++i)
        {
            colIndex = 0;
            for(auto state : booleans->GetAllStates())
            {
                EXPECT_EQ(pNode->_table[i][colIndex],
                          pNode->AccessTable(depValues, state));
                ++colIndex;
            }
        }

        pNode = std::make_shared<BayesianNode>("q", MakeHairColorFunctor(),
            DependencyType{
                {"a", MakeBooleanFunctor()}
            });
        pNode->_table = {
            {0.01,  0.000234,   0.394},
            {0.5,   0.092,      0.55}
        };
        rowIndex = 0;
        depValues.clear();
        for(auto state1 : booleans->GetAllStates())
        {
            colIndex = 0;
            depValues["a"] = state1;
            for(auto state2 : hair->GetAllStates())
            {
                EXPECT_EQ(pNode->_table[rowIndex][colIndex],
                          pNode->AccessTable(depValues, state2));
                ++colIndex;
            }
            ++rowIndex;
        }

        pNode = std::make_shared<BayesianNode>("q", MakeBooleanFunctor(),
            DependencyType{
                {"a", MakeBooleanFunctor()},
                {"b", MakeBooleanFunctor()}
            });
        pNode->_table = {
            {0.1,   0.2},
            {0.3,   0.4},
            {0.5,   0.6},
            {0.7,   0.8}
        };
        rowIndex = 0;
        depValues.clear();
        for(auto state1 : booleans->GetAllStates())
        {
            depValues["a"] = state1;
            for(auto state2 : booleans->GetAllStates())
            {
                depValues["b"] = state2;
                colIndex = 0;
                for(auto state3 : booleans->GetAllStates())
                {
                    EXPECT_EQ(pNode->_table[rowIndex][colIndex],
                              pNode->AccessTable(depValues, state3));
                    ++colIndex;
                }
                ++rowIndex;
            }
        }

        pNode = std::make_shared<BayesianNode>("q", MakeBooleanFunctor(),
            DependencyType{
                {"a", MakeBooleanFunctor()},
                {"b", MakeBooleanFunctor()},
                {"e", MakeEyeColorFunctor()},
                {"d", MakeHairColorFunctor()},
                {"c", MakeEyeColorFunctor()}
            });
        for(int i = 0; i < pNode->_numRows; ++i)
        {
            pNode->_table[i] = {2.0 * i, 2.0 * i + 1.0};
        }
        rowIndex = 0;
        depValues.clear();
        for(auto state1 : eyes->GetAllStates())
        {
            depValues["c"] = state1;
            for(auto state2 : eyes->GetAllStates())
            {
                depValues["e"] = state2;
                for(auto state3 : hair->GetAllStates())
                {
                    depValues["d"] = state3;
                    for(auto state4 : booleans->GetAllStates())
                    {
                        depValues["a"] = state4;
                        for(auto state5 : booleans->GetAllStates())
                        {
                            depValues["b"] = state5;
                            colIndex = 0;
                            for(auto state6 : booleans->GetAllStates())
                            {
                                EXPECT_EQ(pNode->_table[rowIndex][colIndex],
                                    pNode->AccessTable(depValues, state6));
                                ++colIndex;
                            }
                            ++rowIndex;
                        }
                    }
                }
            }
        }

    }

    void BayesianNodeTest::TestNodePrOfState()
    {

    }

} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers
