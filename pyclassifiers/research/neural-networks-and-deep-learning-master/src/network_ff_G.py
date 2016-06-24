
# SYSTEM IMPORTS
import math
import random

# PYTHON PROJECT IMPORTS


def sigmoid(z):
    return 1.0 / (1.0 + math.exp(-z))

def sigmoidPrime(z):
    val = sigmoid(z)
    return val * (1 - val)


class Neuron(object):
    def __init__(self, activationFunction=sigmoid):
        self.a = 0.0
        self.z = 0.0
        self.delta = 0.0
        self.deltaUpdate = 0.0
        self.bias = random.gauss(0, 1)
        self.activationFunction = activationFunction
        self.incomingEdges = []
        self.outgoingEdges = []

    def computeValue(self):
        self.z = 0.0
        for edge in self.incomingEdges:
            self.z += edge.source.a * edge.weight
        self.z += self.bias
        self.a = self.activationFunction(self.z)

    def update(self, learningRate):
        self.bias -= learningRate * self.deltaUpdate
        self.deltaUpdate = 0.0


class Synapse(object):
    def __init__(self, source, dest):
        self.source = source
        self.dest = dest
        self.weight = random.gauss(0, 1)
        self.wUpdate = 0.0

    def update(self, learningRate):
        self.weight -= learningRate * self.wUpdate
        self.wUpdate = 0.0



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
                    inputNeuron.a = values[valueIndex]
                    valueIndex += 1
            elif layerIndex == len(self.layers) - 1:
                for outputNeuron in self.layers[layerIndex]:
                    outputNeuron.computeValue()
                    result.append(outputNeuron.a)
            else:
                for neuron in self.layers[layerIndex]:
                    neuron.computeValue()
        return result

    def costFunction(self, values, expectedResults):
        computedResults = self.feedForward(values)
        error = 0.0
        for computed, theoretical in zip(computedResults, expectedResults):
            error += (computed - theoretical) ** 2
        return error * 0.5

    def costFunctionPrime(self, computed, expected):
        return (computed - expected)

    def backPropogate(self, values, expectedResults):
        self.feedForward(values)
        expectedResultsIndex = 0
        for index in xrange(len(self.layers) - 1, 0, -1):
            # print("Operating on layer: %s" % index)
            if index == len(self.layers) - 1:
                for outputNeuron in self.layers[index]:
                    # compute delta as deriv. of Cost function * sigmoidPrime(z of this layer)
                    outputNeuron.delta = self.costFunctionPrime(outputNeuron.a,
                        expectedResults[expectedResultsIndex]) * sigmoidPrime(outputNeuron.z)
                    # print("\toutputNeuron delta: %s" % outputNeuron.delta)
                    outputNeuron.deltaUpdate += outputNeuron.delta
                    # outputNeuron.bias -= self.learningRate * outputNeuron.delta
                    for synapse in outputNeuron.incomingEdges:
                        synapse.wUpdate += synapse.source.a * outputNeuron.delta
                        # synapse.weight -= self.learningRate * synapse.source.a * outputNeuron.delta

                    expectedResultsIndex += 1
            else:
                for neuron in self.layers[index]:
                    # compute delta as transpose of weight matrix of next layer * delta of next layer
                    # elementwise product with sigmoidPrime(z of this layer)

                    # element wise this just means multiply the next layers delta by the weight
                    # of the edge between the two neurons
                    neuron.delta = 0.0
                    for synapse in neuron.outgoingEdges:
                        neuron.delta += synapse.weight * synapse.dest.delta
                    neuron.delta *= sigmoidPrime(neuron.z)
                    neuron.deltaUpdate += neuron.delta
                    # neuron.bias -= self.learningRate * neuron.delta
                    for synapse in neuron.incomingEdges:
                        synapse.wUpdate += synapse.source.a * neuron.delta
                        # synapse.weight -= self.learningRate * synapse.source.a * neuron.delta

    def miniBatchUpdate(self, miniBatch, miniBatchSize):
        for x, y in miniBatch:
            self.backPropogate(x, y)
        for i in xrange(len(self.layers) - 1, 0, -1):
            for neuron in self.layers[i]:
                for synapse in neuron.incomingEdges:
                    synapse.update(self.learningRate / miniBatchSize)
                neuron.update(self.learningRate / miniBatchSize)

    def argmax(self, vals):
        index = 0
        maxVal = 0.0
        for i in range(len(vals)):
            if vals[i] > maxVal:
                maxVal = vals[i]
                index = i
        return index


    def evaluate(self, test_data):
        """Return the number of test inputs for which the neural
        network outputs the correct result. Note that the neural
        network's output is assumed to be the index of whichever
        neuron in the final layer has the highest activation."""
        test_results = [(self.argmax(self.feedForward(x)), y)
                        for (x, y) in test_data]
        return sum(int(x == y) for (x, y) in test_results)

    def train(self, trainingData, epochs, miniBatchSize, testData=None):
        if testData:
            testDataLength = len(testData)
        n = len(trainingData)
        for j in xrange(epochs):
            random.shuffle(trainingData)
            miniBatches = [
                trainingData[k:k+miniBatchSize]
                for k in xrange(0, n, miniBatchSize)]
            print("numMiniBatches: %s" % (len(miniBatches)))
            it = 0
            for miniBatch in miniBatches:
                self.miniBatchUpdate(miniBatch, miniBatchSize)
                print("Mini batch update: %s done" % it)
                it += 1
            if testData:
                print "Epoch {0}: {1} / {2}".format(
                    j, self.evaluate(testData), testDataLength)
            else:
                print "Epoch {0} complete".format(j)
