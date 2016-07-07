// SYSTEM INCLUDES
#include <algorithm>
#include <cassert>

// C++ PROJECT INCLUDES
#include "Bayesian/BayesianNode.hpp"

namespace Classifiers
{
namespace Bayesian
{

    BayesianNode::BayesianNode(std::string name,
                         StateFunctorPtr nameStateFunctor,
                         DependencyType dependencies) :
        _name(name), _numRows(1), _nameStateFunctor(nameStateFunctor),
        _dependencies(dependencies), _parents(),
        _totalProbabilities(), _table()
    {
        for(auto& dep : this->_dependencies)
        {
            this->_numRows *= std::get<1>(dep)->NumStates();
            this->_parents[std::get<0>(dep)] = nullptr;
        }

        this->GenerateTable(this->_dependencies,
                            this->_nameStateFunctor);

        if(this->_table.size() > 1)
        {
            this->_totalProbabilities.resize(this->_nameStateFunctor->NumStates(),
                                             0.0);
        }
    }

    BayesianNode::~BayesianNode()
    {
    }

    double& BayesianNode::AccessTable(std::map<std::string, std::string>& depStates,
                                      std::string& state)
    {
        return this->_table[this->ValuesToIndex(depStates)]\
        [this->_nameStateFunctor->Hash(state)];
    }

    double& BayesianNode::PrOfState(std::string& state)
    {
        if(this->_numRows > 1)
        {
            return this->_totalProbabilities[this->_nameStateFunctor->Hash(state)];
        }
        return this->_table[0][this->_nameStateFunctor->Hash(state)];
    }

    void BayesianNode::GenerateTable(DependencyType& deps,
                                     StateFunctorPtr& nameStateFunctorPtr)
    {
        std::stable_sort(deps.begin(), deps.end(), [](std::pair<std::string, StateFunctorPtr> left,
                                                      std::pair<std::string, StateFunctorPtr> right)
        {
            return std::get<0>(left) < std::get<0>(right);
        });
        std::stable_sort(deps.begin(), deps.end(), [](std::pair<std::string, StateFunctorPtr> left,
                                                      std::pair<std::string, StateFunctorPtr> right)
        {
            return std::get<1>(left)->NumStates() > std::get<1>(right)->NumStates();
        });

        for(int i = 0; i < this->_numRows; ++i)
        {
            this->_table.push_back(std::vector<double>(this->_nameStateFunctor->NumStates(), 0.0));
        }
    }

    int BayesianNode::ValuesToIndex(std::map<std::string, std::string>& depStates)
    {
        int index = 0;
        int base = 1;

        for(int i = (int)this->_dependencies.size() - 1; i >= 0; --i)
        {
            assert(depStates.find(std::get<0>(this->_dependencies[i])) != depStates.end());

            index += (std::get<1>(this->_dependencies[i])
                ->Hash(depStates[std::get<0>(this->_dependencies[i])])) * base;

            base *= std::get<1>(this->_dependencies[i])->NumStates();
        }
        return index;
    }

} // end of namespace Bayesian
} // end of namespace Classifiers
