
# SYSTEM IMPORTS
import math
import random

# PYTHON PROJECT IMPORTS


def sigmoid(z):
    return 1.0 / (1 + math.exp(-z))

def sigmoidPrime(z):
    val = sigmoid(z)
    return z * (1 - z)


class Neuron(object):
    def __init__(self, activationFunction=sigmoid):
        self.value = 0.0
        self.bias = random.random()
        self.activationFunction = activationFunction
        self.incomingEdges = []
        self.outgoingEdges = []

    def computeValue(self):
        value = 0
        for edge in self.incomingEdges:
            value += edge.source.value * edge.weight + self.bias
        self.value = self.activationFunction(value)


class Synapse(object):
    def __init__(self, source, dest):
        self.source = source
        self.dest = dest
        self.weight = random.random()



class Network(object):
    def __init__(self, layers, learningRate=1):
        self.numLayers = len(layers)
        self.learningRate = learningRate
        # instantiate all the neurons
        self.layers = [ [Neuron() for y in xrange(numInLayer)] for numInLayer in layers]

        # instantiate synapses between neurons
        synapse = None
        for i in xrange(len(self.layers[:-1])):
            for neuron1 in self.layers[i]:
                for neuron2 in self.layers[i + 1]:
                    # add synapses
                    synapse = Synapse(neuron1, neuron2)
                    neuron1.outgoingEdges.append(synapse)
                    neuron2.incomingEdges.append(synapse)

    def feedForward(self, values):
        valueIndex = 0
        result = []
        for layerIndex in xrange(len(self.layers)):
            if layerIndex == 0:
                for inputNeuron in self.layers[layerIndex]:
                    inputNeuron.value = values[valueIndex]
                    valueIndex += 1
            elif layerIndex == len(self.layers) - 1:
                for outputNeuron in self.layers[layerIndex]:
                    outputNeuron.computeValue()
                    result.append(outputNeuron.value)
            else:
                for neuron in self.layers[layerIndex]:
                    neuron.computeValue()
        return result

    def costFunction(self, values, expectedResults):
        computedResults = self.feedForward(values)
        error = 0.0
        for computed, theoretical in zip(computedResults, expectedResults):
            error += abs(theoretical - computed) ** 2
        return math.sqrt(error) * 0.5

    def backPropogate(self, values):
        pass

