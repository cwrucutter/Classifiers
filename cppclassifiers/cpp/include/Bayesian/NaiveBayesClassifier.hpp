#pragma once
#ifndef CLASSIFIERS_BAYESIAN_NAIVEBAYESCLASSIFIER_HPP
#define CLASSIFIERS_BAYESIAN_NAIVEBAYESCLASSIFIER_HPP

#ifndef NAIVEBAYESCLASSIFIER_TEST_FRIENDS
#define NAIVEBAYESCLASSIFIER_TEST_FRIENDS
#endif


// SYSTEM INCLUDES
#include <map>
#include <memory>
#include <string>
#include <vector>

// C++ PROJECT INCLUDES
#include "Bayesian/StateFunctor.hpp"
#include "Bayesian/BayesianNode.hpp"

namespace Classifiers
{
namespace Bayesian
{

    class NaiveBayesClassifier
    {
        NAIVEBAYESCLASSIFIER_TEST_FRIENDS;
        public:

            NaiveBayesClassifier(std::string varName,
                StateFunctorPtr varNameFunctor, std::map<std::string,
                    StateFunctorPtr> featureNamesAndStates);

            ~NaiveBayesClassifier();

            void Train(std::vector<
                std::map<std::string, std::string> >& trainingData);


            std::vector<std::string> Classify(std::map<std::string, std::string>& features);


        private:

            double PrOfState(std::map<std::string, std::string>& features,
                std::string& state);

            std::string                             _varName;
            StateFunctorPtr                         _varNameFunctor;
            std::map<std::string, BayesianNodePtr>  _nodes;
            std::vector<std::string>                _featureNames;

    };

    using NaiveBayesClassifierPtr = std::shared_ptr<NaiveBayesClassifier>;

} // end of namespace Bayesian
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_BAYESIAN_NAIVEBAYESCLASSIFIER_HPP
