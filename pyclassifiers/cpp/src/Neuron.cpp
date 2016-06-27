// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Neuron.hpp"
#include "Synapse.hpp"

Neuron::Neuron(PyObject* pActivFunc) : _a(0.0), _z(0.0),
    _delta(0.0), _deltaUpdate(0.0), _bias(0.0 /**make random value*/),
    _pActivationFunction(pActivFunc)
{
    Py_INCREF(this->_pActivationFunction);
}

Neuron::~Neuron()
{
    Py_DECREF(this->_pActivationFunction);
}

void Neuron::ComputeValue()
{
    this->_z = 0.0;
    for(SynapsePtr pSynapse: this->_incomingEdges) 
    {
        self._z += pSynapse.source._a * pSynapse._weight;
    }
    this->_z += this->_bias;

    // python conversion
    PyObject* arg = Py_BuildValue("d", this->_z);
    PyObject* result = PyObject_CallObject(this->_pActivationFunction, arg);
    Py_DECREF(arg);

    PyArg_ParseTuple(result, "d", &this->_a);
    Py_DECREF(result);
    // end of python conversion
}

void Neuron::Update(double learningRate)
{
    this->_bias -= learningRate * this->_deltaUpdate;
    this->_deltaUpdate = 0.0;
}
