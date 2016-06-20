#!/usr/bin/env python

# SYSTEM IMPORTS
import unittest
import rospy
import rostest
import sys
import os



currentDir = os.path.dirname(os.path.realpath(__file__))
scriptsDir = os.path.abspath(os.path.join(currentDir, "..", ".."))
sys.path.extend([currentDir, scriptsDir])

# PYTHON PROJECT IMPORTS
import NaiveBayesClassifier
import TestFunctors


class NaiveBayesClassifierTest(unittest.TestCase):

    def test_constructor(self):
        testClassifier = NaiveBayesClassifier.NaiveBayesClassifier("lion", TestFunctors.BooleanFunctor(),
            {"has-fur?": TestFunctors.BooleanFunctor(),
            "long-teeth?": TestFunctors.BooleanFunctor(),
            "scary?": TestFunctors.BooleanFunctor()})

        self.assertEqual("lion", testClassifier.classificationName)
        self.assertEqual(TestFunctors.BooleanFunctor(), testClassifier.classificationStateFunctor)
        self.assertEqual(4, len(testClassifier.nodes))
        for featureName in ["has-fur?", "long-teeth?", "scary?"]:
            self.assertTrue(featureName in testClassifier.featureNames)

    def test_train(self):
        testClassifier = NaiveBayesClassifier.NaiveBayesClassifier("lion", TestFunctors.BooleanFunctor(),
            {"has-fur?": TestFunctors.BooleanFunctor(),
            "long-teeth?": TestFunctors.BooleanFunctor(),
            "scary?": TestFunctors.BooleanFunctor()})

        sampleTrainingData = [
            {"has-fur?":True, "long-teeth?":False, "scary?":False, "lion": False},
            {"has-fur?":False, "long-teeth?":True, "scary?":True, "lion": False},
            {"has-fur?":True, "long-teeth?":True, "scary?":True, "lion": True}
        ]
        testClassifier.train(sampleTrainingData)

        lionNode = testClassifier.nodes["lion"]
        hasFurNode = testClassifier.nodes["has-fur?"]
        longTeethNode = testClassifier.nodes["long-teeth?"]
        scaryNode = testClassifier.nodes["scary?"]
        conditionalsTable = [[0.5, 0.5], [0, 1]]
        self.assertEqual([0.67, 0.33], [round(x, 2) for x in lionNode.table[0]])
        self.assertEqual(conditionalsTable, [[round(y, 2) for y in x] for x in hasFurNode.table])
        self.assertEqual(conditionalsTable, [[round(y, 2) for y in x] for x in longTeethNode.table])
        self.assertEqual(conditionalsTable, [[round(y, 2) for y in x] for x in scaryNode.table])

    def test_prOfState(self):
        testClassifier = NaiveBayesClassifier.NaiveBayesClassifier("lion", TestFunctors.BooleanFunctor(),
            {"has-fur?": TestFunctors.BooleanFunctor(),
            "long-teeth?": TestFunctors.BooleanFunctor(),
            "scary?": TestFunctors.BooleanFunctor()})
        testClassifier.nodes["lion"].table = [(0.9, 0.1)]
        testClassifier.nodes["has-fur?"].table = [(0.9, 0.1),
                                                  (0.5, 0.5)]
        testClassifier.nodes["long-teeth?"].table = [(0.5, 0.5),
                                                     (0.1, 0.9)]
        testClassifier.nodes["scary?"].table = [(0.5, 0.5),
                                                (0.2, 0.8)]

        featureVector = {"has-fur?": True, "long-teeth?": False, "scary?": False}
        self.assertEqual(0.001, round(testClassifier.prOfState(featureVector, True), 3))
        self.assertEqual(0.0225, round(testClassifier.prOfState(featureVector, False), 4))

    def test_classify(self):
        testClassifier = NaiveBayesClassifier.NaiveBayesClassifier("lion", TestFunctors.BooleanFunctor(),
            {"has-fur?": TestFunctors.BooleanFunctor(),
            "long-teeth?": TestFunctors.BooleanFunctor(),
            "scary?": TestFunctors.BooleanFunctor()})
        testClassifier.nodes["lion"].table = [(0.9, 0.1)]
        testClassifier.nodes["has-fur?"].table = [(0.1, 0.9),
                                                  (0.5, 0.5)]
        testClassifier.nodes["long-teeth?"].table = [(0.5, 0.5),
                                                     (0.1, 0.9)]
        testClassifier.nodes["scary?"].table = [(0.5, 0.5),
                                                (0.2, 0.8)]

        self.assertEqual([False], testClassifier.classify(
            {"has-fur?": True, "long-teeth?": False, "scary?": False}))


if __name__ == "__main__":
    rostest.rosrun("pyclassifiers", "NaiveBayesClassifierTest", NaiveBayesClassifierTest)
