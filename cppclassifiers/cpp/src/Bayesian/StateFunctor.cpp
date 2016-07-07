// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Bayesian/StateFunctor.hpp"

namespace Classifiers
{
namespace Bayesian
{

    StateFunctor::StateFunctor(std::vector<std::string> states) :
        _allStates(states)
    {
    }

    StateFunctor::~StateFunctor()
    {
    }

    int StateFunctor::Hash(const std::string& object)
    {
        for(int i = 0; i < this->_allStates.size(); ++i)
        {
            if(this->_allStates[i] == object)
            {
                return i;
            }
        }
        return -1;
    }

    int StateFunctor::NumStates()
    {
        return (int)this->_allStates.size();
    }

    std::vector<std::string>& StateFunctor::GetAllStates()
    {
        return this->_allStates;
    }

} // end of namespace Bayesian
} // end of namespace Classifiers
