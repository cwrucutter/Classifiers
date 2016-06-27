#pragma once
#ifndef CLASSIFIERS_NEURAL_CPPNEURON_HPP
#define CLASSIFIERS_NEURAL_CPPNEURON_HPP


// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Neural/INeuron.hpp"

namespace Classifiers
{
namespace Neural
{

    class CppNeuron : public INeuron
    {
        public:

            CppNeuron(double (*pFunc)(double));

            ~CppNeuron();

            virtual void ComputeValue();

        private:

            double (*_pActivationFunction)(double);

    };

    using CppNeuronPrt = std::shared_ptr<CppNeuron>;

} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_CPPNEURON_HPP
