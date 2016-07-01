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

            CppNeuron(double (*pActiveFunc)(double),
                      double (*pActiveFuncPrime)(double));

            ~CppNeuron();

            virtual void ComputeValue() override;

            virtual double ActivationFunction(double val) override;

            virtual double ActivationFunctionPrime(double val) override;

        private:

            double (*_pActivationFunction)(double);
            double (*_pActivationFunctionPrime)(double);

    };

    using CppNeuronPtr = std::shared_ptr<CppNeuron>;

} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_CPPNEURON_HPP
