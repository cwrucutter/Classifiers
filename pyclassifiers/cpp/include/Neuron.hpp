#pragma once
#ifndef PYCLASSIFIERS_NEURON_HPP
#define PYCLASSIFIERS_NEURON_HPP


// SYSTEM INCLUDES
#include <Python.h>
#include <memory>
#include <vector>


// C++ PROJECT INCLUDES


struct Synapse;
using SynapsePtr = std::shared_ptr<Synapse>;

class Neuron
{
    private:

        double _a, _z, _delta, _deltaUpdate, _bias;

        PyObject* _pActivationFunction;

        std::vector<SynapsePtr> _incomingEdges;
        std::vector<SynapsePtr> _outgoingEdges;

    public:

        Neuron(PyObject* pActivFunc);

        ~Neuron();

        void ComputeValue();

        void Update(double learningRate);
};

#endif
