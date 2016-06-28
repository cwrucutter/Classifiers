#pragma once
#ifndef CLASSIFIERS_NEURAL_CPPNEURON_HPP
#define CLASSIFIERS_NEURAL_CPPNEURON_HPP

#ifndef TEST_CPPNEURON_FRIENDS
#define TEST_CPPNEURON_FRIENDS
#endif

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Neural/INeuron.hpp"

namespace Classifiers
{
namespace Neural
{

    class CppNeuron : public INeuron
    {
        TEST_CPPNEURON_FRIENDS;
        public:

            CppNeuron(double (*pFunc)(double));

            ~CppNeuron();

            virtual void ComputeValue();

        private:

            double (*_pActivationFunction)(double);

    };

    using CppNeuronPtr = std::shared_ptr<CppNeuron>;

} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_CPPNEURON_HPP
