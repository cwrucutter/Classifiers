#pragma once
#ifndef CLASSIFIERS_BAYESIAN_STATEFUNCTOR_HPP
#define CLASSIFIERS_BAYESIAN_STATEFUNCTOR_HPP


// SYSTEM INCLUDES
#include <memory>
#include <vector>

// C++ PROJECT INCLUDES

namespace Classifiers
{
namespace Bayesian
{

    class StateFunctor
    {
        public:

            StateFunctor(std::vector<std::string> states);
            ~StateFunctor();

            int Hash(const std::string& object);

            int NumStates();;

            std::vector<std::string>& GetAllStates();

        protected:

            std::vector<std::string>    _allStates;

    };

    using StateFunctorPtr = std::shared_ptr<StateFunctor>;

} // end of namespace Bayesian
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_BAYESIAN_STATEFUNCTOR_HPP
