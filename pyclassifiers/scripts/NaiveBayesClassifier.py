
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
        # make the giant table that we will be filling out
        # only want to go through the data once (if possible)
        # create a dictionary....keys will be a featureVector
        # handle classificationNode separatley

        # want to have (numerator, denominator)
        '''
        classificationNameData = tuple(0 for x in range(len(self.classificationStateFunctor) + 1))
        featureData = []
        for featureIndex1 in range(len(self.featureNames)):
            featureData.append([])
            for classificationStateIndex in range(len(self.classificationStateFunctor.getAllStates())):
                featureData[featureIndex1].append([0 for x in 
                    len(self.nodes[self.featureNames[featureIndex1]].table)])

        for trainingTupleDict in trainingData:
            # update training tuple with new training data
            nameEntry = trainingTupleDict[self.classificationName]
            # update denominator
            classificationNameData[len(self.classificationStateFunctor)] += 1
            # update numerator
            classificationNameData[self.classificationStateFunctor.hash(nameEntry)] += 1
            for feature in trainingTupleDict.items():
                if feature[0] == self.classificationName:
                    continue
                else:
                    # update the value of the tuple
                    featureData[feature[0]]\
                        [self.classificationStateFunctor.hash(nameEntry)]\
                            [self.nodes[feature[0]].nameStateFunctor.hash(feature[1])] += 1

        '''

        # for every element in trainingData
        assert isinstance(trainingData, list)
        # print("COUNTING")
        for trainingTupleDict in trainingData:
            assert isinstance(trainingTupleDict, dict)

            # for every feature in each element of trainingData
            for feature in trainingTupleDict.items():
                # print("Found feature: %s with value: %s" % (feature))
                node = self.nodes[feature[0]]
                # print("node has number of rows: %s" % len(node.table))
                functor = node.nameStateFunctor
                # get the appropriate row for the values given
                if feature[0] == self.classificationName:
                    index = node.valuesToIndex({})
                else:
                    index = node.valuesToIndex({self.classificationName:
                        trainingTupleDict[self.classificationName]})
                node.table[index][functor.hash(feature[1])] += 1

        # handle the classificationNode at the end.
        # print("NORMALIZING")
        rootNode = self.nodes[self.classificationName]
        for node in self.nodes.items():
            # print("name: %s" % node[0])
            # matrixString = ""
            # for row in range(len(node[1].table)):
                # matrixString += "\t["
                # for column in range(len(node[1].table[row])):
                    # matrixString += ('%s, ' % node[1].table[row][column])
                # matrixString += "]\n"
            # print(matrixString)
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

        '''
        print("POST NORMALIZING")
        for node in self.nodes.items():
            print("name: %s" % node[0])
            matrixString = ""
            for row in range(len(node[1].table)):
                matrixString += "\t["
                for column in range(len(node[1].table[row])):
                    matrixString += ("%s, " % node[1].table[row][column])
                matrixString += "]\n"
            print(matrixString)
        '''

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
        return solutions
