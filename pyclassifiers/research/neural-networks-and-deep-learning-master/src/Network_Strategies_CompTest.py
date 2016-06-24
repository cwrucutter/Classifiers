
# SYSTEM IMPORTS
import unittest
import numpy as np

# PYTHON PROJECT IMPORTS
import network as nm
import network_ff_G as ng

class NetworkStrategiesCompTests(unittest.TestCase):
    def test_feedForward(self):
        # test with default [2,3,1] network
        default_network_scheme = [2, 3, 1]
        net1 = nm.Network(default_network_scheme)
        net2 = ng.Network(default_network_scheme)

        self.assertEqual(net1.num_layers, len(net2.layers))
        net1Weights = net1.weights
        net1Biases = net1.biases

        for layerIndex in range(1, len(net2.layers)):
            for neuronIndex in range(len(net2.layers[layerIndex])):
                for edgeIndex in range(len(net2.layers[layerIndex][neuronIndex].incomingEdges)):
                    net2.layers[layerIndex][neuronIndex].incomingEdges[edgeIndex].weight = \
                        net1Weights[layerIndex][neuronIndex][edgeIndex]
                net2.layers[layerIndex][neuronIndex].bias = net1Biases[layerIndex][neuronIndex]

        self.assertEqual(net1.feedforward(np.array([[0],[1]])), numpy.array([net2.feedforward([0,1])]))

if __name__ == '__main__':
    unittest.main()
