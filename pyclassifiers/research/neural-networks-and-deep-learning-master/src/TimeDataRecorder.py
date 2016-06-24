
# SYSTEM IMPORTS
import json
import os
import sys
import time

# PYTHON PROJECT IMPORTS
import network
import network_ff_G


currentDir = os.path.dirname(os.path.realpath(__file__))

class TimeRecorder(object):
    def __init__(self, netSizes, learningRate=1, 
        filePath=os.path.join(currentDir, "NeuralNetRuntimes.json")):
        if os.path.exists(filePath):
            with open(filePath, "r") as file:
                self.data = json.load(file)
        else:
            self.data = {
                "matrix": {},
                "graph": {},
            }
        self.filePath = filePath
        self.netSizesStr = "["
        for i in range(len(netSizes)):
            self.netSizesStr += ("%s" % netSizes[i])
            if i < len(netSizes) - 1:
                self.netSizesStr += ", "
        self.netSizesStr += "]"
        self.numpyNet = network.Network(netSizes)
        self.graphNet = network_ff_G.Network(netSizes, learningRate)
        self.learningRate = learningRate

    def time(self, trainingData, epochs, miniBatchSize, testData=None):
        start = time.clock()
        self.numpyNet.SGD(trainingData, epochs, miniBatchSize, self.learningRate, testData)
        diff = (time.clock() - start)

        if self.netSizesStr not in self.data["matrix"]:
            self.data["matrix"][self.netSizesStr] = {}
        if epochs not in self.data["matrix"][self.netSizesStr]:
            self.data["matrix"][self.netSizesStr][epochs] = [diff]
        else:
            self.data["matrix"][self.netSizesStr][epochs].append(diff)
        start = time.clock()
        self.graphNet.train(trainingData, epochs, miniBatchSize, testData)
        diff = (time.clock() - start)

        if self.netSizesStr not in self.data["graph"]:
            self.data["graph"][self.netSizesStr] = {}
        if epochs not in self.data["graph"][self.netSizesStr]:
            self.data["graph"][self.netSizesStr][epochs] = [diff]
        else:
            self.data["graph"][self.netSizesStr][epochs].append(diff)

        with open(self.filePath, "w") as file:
            json.dump(self.data, file)



