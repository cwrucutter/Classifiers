// SYSTEM INCLUDES
#include <gtest/gtest.h>
#include <map>
#include <memory>
#include <random>

// C++ PROJECT INCLUDES
#include "Bayesian/NaiveBayesClassifier.hpp"
#include "unitTest/Common/TestStateFunctors.hpp"

namespace Classifiers
{
namespace Bayesian
{
namespace Tests
{

    std::vector<std::map<std::string, std::string> >    naiveTrainingData;
    std::vector<std::map<std::string, std::string> >    naiveTestData;
    NaiveBayesClassifierPtr                             pClassifier;
    std::map<std::string, StateFunctorPtr>              varMap;
    static const char alphaNum[] = 
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static const StateFunctorPtr functors[] = {
        MakeBooleanFunctor(),
        MakeHairColorFunctor(),
        MakeEyeColorFunctor()
    };

    TEST(NaiveBayesClassifierSpeedTest, InitDataAndClassifier)
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<double> unif(0, 699);
        std::uniform_real_distribution<double> unif2(0, 2);

        for(int i = 0; i < 700; ++i)
        {
            std::string name(700, 'a');
            for(int j = 0; j < 700; ++j)
            {
                name[j] = alphaNum[((int)unif(generator))];
            }
            varMap[name] = functors[(int)unif2(generator)];
        }
        pClassifier = std::make_shared<NaiveBayesClassifier>("q",
            MakeEyeColorFunctor(), varMap);

        // make the training data
        int trainingDataSize = 50000;
        int testDataSize = 10000;
        naiveTrainingData.reserve(trainingDataSize);
        naiveTestData.reserve(testDataSize);
        std::uniform_real_distribution<double> eyeDist(0, 3);
        for(int i = 0; i < trainingDataSize; ++i)
        {
            std::map<std::string, std::string> featureMap;
            for(auto it = varMap.begin(); it != varMap.end(); ++it)
            {
                std::uniform_real_distribution<double> stateDist(0, it->second->NumStates());
                featureMap[it->first] = it->second->GetAllStates()[(int)stateDist(generator)];
            }
            featureMap["q"] = functors[2]->GetAllStates()[(int)eyeDist(generator)];
            naiveTrainingData.push_back(featureMap);
        }

        for(int i = 0; i < testDataSize; ++i)
        {
            std::map<std::string, std::string> featureMap;
            for(auto it = varMap.begin(); it != varMap.end(); ++it)
            {
                std::uniform_real_distribution<double> stateDist(0, it->second->NumStates());
                featureMap[it->first] = it->second->GetAllStates()[(int)stateDist(generator)];
            }
            naiveTestData.push_back(featureMap);
        }
    }

    TEST(NaiveBayesClassifierSpeedTest, TrainSpeedTest)
    {
        pClassifier->Train(naiveTrainingData);
    }

    TEST(NaiveBayesClassifierSpeedTest, ClassifySpeedTest)
    {
        for(auto& featureVector : naiveTestData)
        {
            pClassifier->Classify(featureVector);
        }
    }



} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers
