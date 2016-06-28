#pragma once
#ifndef CLASSIFIERS_TESTS_FRIENDDECLARATIONS_HPP
#define CLASSIFIERS_TESTS_FRIENDDECLARATIONS_HPP

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    class CppNeuronTest;
    class NetTestCppNeuron;
    #define TEST_CPPNEURON_FRIENDS \
        friend class Classifiers::Neural::Tests::CppNeuronTest;\
        friend class Classifiers::Neural::Tests::NetTestCppNeuron;

    class FFNeuralNetTest;
    #define TEST_NET_FRIENDS \
        friend class Classifiers::Neural::Tests::FFNeuralNetTest;

} // end of namespace Classifiers
} // end of namespace Neural
} // end of namespace Classifiers


#endif // end of CLASSIFIERS_TESTS_FRIENDDECLARATIONS_HPP
