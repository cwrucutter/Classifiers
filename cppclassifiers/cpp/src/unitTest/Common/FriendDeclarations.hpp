#pragma once
#ifndef CLASSIFIERS_TESTS_FRIENDDECLARATIONS_HPP
#define CLASSIFIERS_TESTS_FRIENDDECLARATIONS_HPP

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    class NeuronTest;
    class NetTestNeuron;
    #define TEST_NEURON_FRIENDS \
        friend class Classifiers::Neural::Tests::NeuronTest;\
        friend class Classifiers::Neural::Tests::NetTestNeuron;

    class FFNeuralNetTest;
    #define TEST_NET_FRIENDS \
        friend class Classifiers::Neural::Tests::FFNeuralNetTest;

} // end of namespace Tests
} // end of namespace Neural


namespace Bayesian
{
namespace Tests
{

    class BayesianNodeTest;
    #define BAYESIANNODE_TEST_FRIENDS \
        friend class Classifiers::Bayesian::Tests::BayesianNodeTest;

} // end of namespace Tests
} // end of namespace Bayesian
} // end of namespace Classifiers


#endif // end of CLASSIFIERS_TESTS_FRIENDDECLARATIONS_HPP
