#pragma once
#ifndef CLASSIFIERS_BAYESIAN_BAYESIANNODE_HPP
#define CLASSIFIERS_BAYESIAN_BAYESIANNODE_HPP


#ifndef BAYESIANNODE_TEST_FRIENDS
#define BAYESIANNODE_TEST_FRIENDS
#endif


// SYSTEM INCLUDES
#include <map>
#include <memory>
#include <string>

// C++ PROJECT INCLUDES
#include "Bayesian/StateFunctor.hpp"

namespace Classifiers
{
namespace Bayesian
{

    class NaiveBayesClassifier;

    class BayesianNode;
    using BayesianNodePtr = std::shared_ptr<BayesianNode>;

    using DependencyType = std::vector<std::pair<std::string,
                                                 StateFunctorPtr> >;

    class BayesianNode
    {
        friend class NaiveBayesClassifier;
        BAYESIANNODE_TEST_FRIENDS;
        public:

            BayesianNode(std::string name,
                         StateFunctorPtr nameStateFunctor,
                         DependencyType dependencies);
            ~BayesianNode();

            double& AccessTable(std::map<std::string, std::string>& depStates,
                                std::string& state);

            double& PrOfState(std::string& state);

        private:

            void GenerateTable(DependencyType& deps,
                               StateFunctorPtr& nameStateFunctorPtr);

            int ValuesToIndex(std::map<std::string, std::string>& depStates);

            std::string                                 _name;
            int                                         _numRows;
            StateFunctorPtr                             _nameStateFunctor;
            DependencyType                              _dependencies;
            std::map<std::string, BayesianNodePtr>      _parents;
            std::vector<double>                         _totalProbabilities;
            std::vector<std::vector<double> >           _table;

    };

} // end of namespace Bayesian
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_BAYESIAN_BAYESIANNODE_HPP
