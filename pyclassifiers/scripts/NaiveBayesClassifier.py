
# SYSTEM IMPORTS


# PYTHON PROJECT IMPORTS
from StateFunctor import StateFunctor
from BayesianNode import BayesianNode

class NaiveBayesClassifier(object):
    def __init__(self, classificationName, classificationStateFunctor,
                 featureStatesAndNames):
        assert isinstance(featureStatesAndNames, dict)
        assert issubclass(classificationStateFunctor.__class__, StateFunctor)

        self.classificationName = classificationName
        self.classificationStateFunctor = classificationStateFunctor
        self.nodes = {}
        self.nodes[classificationName] = BayesianNode(classificationName,
            classificationStateFunctor, [])
        self.featureNames = []
        for featureInfo in featureStatesAndNames.items():
            self.featureNames.append(featureInfo[0])
            self.nodes[featureInfo[0]] = BayesianNode(featureInfo[0],
                featureInfo[1], [(classificationName, classificationStateFunctor)])

    def train(self, trainingData):
        # for every element in trainingData
        assert isinstance(trainingData, list)
        for trainingTupleDict in trainingData:
            assert isinstance(trainingTupleDict, dict)

            # for every feature in each element of trainingData
            for feature in trainingTupleDict.items():
                node = self.nodes[feature[0]]
                functor = node.nameStateFunctor
                # get the appropriate row for the values given
                if feature[0] == self.classificationName:
                    index = node.valuesToIndex({})
                else:
                    index = node.valuesToIndex({self.classificationName:
                        trainingTupleDict[self.classificationName]})
                node.table[index][functor.hash(feature[1])] += 1

        # handle the classificationNode at the end.
        rootNode = self.nodes[self.classificationName]
        for node in self.nodes.items():
            if node[0] == self.classificationName:
                continue
            else:
                # for every feature that this node has
                for feature in node[1].nameStateFunctor.getAllStates():
                    # for every classification state that its conditioned on (just states of root)
                    for classificationState in self.classificationStateFunctor.getAllStates():
                        node[1].table[node[1].valuesToIndex({self.classificationName: classificationState})]\
                            [node[1].nameStateFunctor.hash(feature)]\
                                /= float(rootNode.accessTable({}, classificationState))

        for i in range(len(rootNode.table[0])):
            rootNode.table[0][i] /= float(len(trainingData))

    def prOfState(self, featureValues, stateValue):
        prob = self.nodes[self.classificationName].pr(stateValue)
        # print("prob: %f" % prob)
        for feature in self.featureNames:
            # print("value: %f" % self.nodes[feature].accessTable(
            #     {self.classificationName: stateValue}, featureValues[feature]))
            prob *= self.nodes[feature].accessTable(
                {self.classificationName: stateValue}, featureValues[feature])
        return prob

    def classify(self, featureValues):
        assert isinstance(featureValues, dict)

        maxPr = 0
        classifications = []
        prOfClassification = 0
        allStates = self.classificationStateFunctor.getAllStates()
        for stateIndex in range(len(allStates)):
            prOfClassification = self.prOfState(featureValues, allStates[stateIndex])
            if prOfClassification >= maxPr:
                maxPr = prOfClassification
                classifications.append((prOfClassification, stateIndex))

        solutions = []
        for i in range(len(classifications) - 1, -1, -1):
            if classifications[i][0] == maxPr:
                solutions.append(allStates[classifications[i][1]])
            else:
                return solutions
        return solutions
