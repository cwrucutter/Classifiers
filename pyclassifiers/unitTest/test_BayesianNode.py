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
import BayesianNode
import TestFunctors

class BayesianNodeTest(unittest.TestCase):

    def test_constructor(self):
        # test 0 dependencies
        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(), [])
        self.assertEqual("testName", testNode.name)
        self.assertEqual(0, len(testNode.dependencies))
        self.assertEqual([], testNode.dependencies)
        self.assertEqual(1, len(testNode.table))
        self.assertEqual([(0, 0)], testNode.table)

        testNode = BayesianNode.BayesianNode("testName", TestFunctors.EyeColorFunctor(), [])
        self.assertEqual([(0, 0, 0, 0)], testNode.table)

        # test 1 input
        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(),
                                             [("a", TestFunctors.EyeColorFunctor())])
        self.assertEqual("testName", testNode.name)
        self.assertEqual(1, len(testNode.dependencies))
        self.assertEqual([("a", TestFunctors.EyeColorFunctor())], testNode.dependencies)
        self.assertEqual(4, len(testNode.table))
        self.assertEqual([(0, 0), (0, 0), (0, 0), (0,0)], testNode.table)

        testNode = BayesianNode.BayesianNode("testName", TestFunctors.HairColorFunctor(),
                                              [("a", TestFunctors.EyeColorFunctor())])
        self.assertEqual(1, len(testNode.dependencies))
        self.assertEqual([("a", TestFunctors.EyeColorFunctor())], testNode.dependencies)
        self.assertEqual(4, len(testNode.table))
        self.assertEqual([(0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0)], testNode.table)

        # test 2 input
        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(),
                                             [
                                              ("b", TestFunctors.EyeColorFunctor()),
                                              ("a", TestFunctors.HairColorFunctor())
                                             ])
        self.assertEqual("testName", testNode.name)
        self.assertEqual(2, len(testNode.dependencies))
        self.assertEqual([("b", TestFunctors.EyeColorFunctor()), ("a", TestFunctors.HairColorFunctor())], \
            testNode.dependencies)
        self.assertEqual(12, len(testNode.table))
        self.assertEqual([(0, 0) for i in range(12)], testNode.table)

        # test many (5)
        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(),
            [("a", TestFunctors.BooleanFunctor()), ("b", TestFunctors.BooleanFunctor()),
             ("e", TestFunctors.EyeColorFunctor()), ("d", TestFunctors.HairColorFunctor()),
             ("c", TestFunctors.EyeColorFunctor())])
        self.assertEqual("testName", testNode.name)
        self.assertEqual(5, len(testNode.dependencies))
        self.assertEqual([("c", TestFunctors.EyeColorFunctor()), ("e", TestFunctors.EyeColorFunctor()),
                          ("d", TestFunctors.HairColorFunctor()), ("a", TestFunctors.BooleanFunctor()),
                          ("b", TestFunctors.BooleanFunctor())], testNode.dependencies)
        self.assertEqual(192, len(testNode.table))
        self.assertEqual([(0, 0) for x in range(192)], testNode.table)

    def test_valuesToIndex(self):
        # test 0 input
        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(), [])
        self.assertEqual(0, testNode.valuesToIndex({}))

        testNode = BayesianNode.BayesianNode("testName", TestFunctors.EyeColorFunctor(), [])
        self.assertEqual(0, testNode.valuesToIndex({}))

        # test 1
        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(),
                                             [("a", TestFunctors.EyeColorFunctor())])
        eyeColors = TestFunctors.EyeColorFunctor().getAllStates()
        self.assertEqual(0, testNode.valuesToIndex({"a": eyeColors[0]}))
        self.assertEqual(1, testNode.valuesToIndex({"a": eyeColors[1]}))
        self.assertEqual(2, testNode.valuesToIndex({"a": eyeColors[2]}))
        self.assertEqual(3, testNode.valuesToIndex({"a": eyeColors[3]}))

        # test 2
        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(),
                                             [("a", TestFunctors.BooleanFunctor()), ("b",
                                              TestFunctors.BooleanFunctor())])

        self.assertEqual(0, testNode.valuesToIndex({"a": False, "b": False}))
        self.assertEqual(1, testNode.valuesToIndex({"a": False, "b": True}))
        self.assertEqual(2, testNode.valuesToIndex({"a": True, "b": False}))
        self.assertEqual(3, testNode.valuesToIndex({"a": True, "b": True}))

        testNode = BayesianNode.BayesianNode("testName", TestFunctors.EyeColorFunctor(),
                                             [
                                                ("a", TestFunctors.EyeColorFunctor()),
                                                ("b", TestFunctors.HairColorFunctor())
                                             ])
        eyes = TestFunctors.EyeColorFunctor().getAllStates()
        hair = TestFunctors.HairColorFunctor().getAllStates()

        self.assertEqual(0, testNode.valuesToIndex({"a":eyes[0], "b":hair[0]}))
        self.assertEqual(1, testNode.valuesToIndex({"a":eyes[0], "b":hair[1]}))
        self.assertEqual(2, testNode.valuesToIndex({"a":eyes[0], "b":hair[2]}))
        self.assertEqual(3, testNode.valuesToIndex({"a":eyes[1], "b":hair[0]}))
        self.assertEqual(4, testNode.valuesToIndex({"a":eyes[1], "b":hair[1]}))
        self.assertEqual(5, testNode.valuesToIndex({"a":eyes[1], "b":hair[2]}))
        self.assertEqual(6, testNode.valuesToIndex({"a":eyes[2], "b":hair[0]}))
        self.assertEqual(7, testNode.valuesToIndex({"a":eyes[2], "b":hair[1]}))
        self.assertEqual(8, testNode.valuesToIndex({"a":eyes[2], "b":hair[2]}))
        self.assertEqual(9, testNode.valuesToIndex({"a":eyes[3], "b":hair[0]}))
        self.assertEqual(10, testNode.valuesToIndex({"a":eyes[3], "b":hair[1]}))
        self.assertEqual(11, testNode.valuesToIndex({"a":eyes[3], "b":hair[2]}))

        # test 3
        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(),
            [("x", TestFunctors.HairColorFunctor()), ("y", TestFunctors.BooleanFunctor()),
             ("z", TestFunctors.BooleanFunctor())])
        booleans = TestFunctors.BooleanFunctor().getAllStates()
        index = 0
        for h in hair:
            for b_1 in booleans:
                for b_2 in booleans:
                    self.assertEqual(index, testNode.valuesToIndex({"x":h, "y":b_1, "z":b_2}))
                    index += 1

        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(),
            [("x", TestFunctors.HairColorFunctor()), ("y", TestFunctors.BooleanFunctor()),
             ("z", TestFunctors.EyeColorFunctor())])
        booleans = TestFunctors.BooleanFunctor().getAllStates()
        index = 0
        print(testNode.dependencies)
        for e in eyes:
            for h in hair:
                for b in booleans:
                    print("z: %s,\tx: %s,\ty: %s" % (e, h, b))
                    self.assertEqual(index, testNode.valuesToIndex({"x":h, "y":b, "z":e}))
                    index += 1

        # test many (5)
        testNode = BayesianNode.BayesianNode("testName", TestFunctors.BooleanFunctor(),
            [("a", TestFunctors.BooleanFunctor()), ("b", TestFunctors.BooleanFunctor()),
             ("e", TestFunctors.EyeColorFunctor()), ("d", TestFunctors.HairColorFunctor()),
             ("c", TestFunctors.EyeColorFunctor())])
        booleans = TestFunctors.BooleanFunctor().getAllStates()
        index = 0
        for eye_1 in eyes:
            for eye_2 in eyes:
                for h in hair:
                    for b_1 in booleans:
                        for b_2 in booleans:
                            self.assertEqual(index,
                                testNode.valuesToIndex({"a":b_1, "b":b_2,
                                    "c":eye_1, "d":h, "e":eye_2}))
                            index += 1

'''
    def test_accessTable(self):
        # test 0
        testNode = BayesianNode.BayesianNode("q", [])
        testNode.table = [#Pr(q=F), Pr(q=T)
                            (0.7,     0.3)]
        self.assertEqual(0.7, testNode.accessTable({}, False))
        self.assertEqual(0.3, testNode.accessTable({}, True))

        # test 1
        testNode = BayesianNode.BayesianNode("q", ["a"])
        testNode.table = [# Pr(q=F | a), Pr(q=T | a)
                                 (0.01,        0.000234), # a:F
                                 (0.5,         0.092)]    # a:T
        self.assertEqual(0.01, testNode.accessTable({"a": False}, False))
        self.assertEqual(0.000234, testNode.accessTable({"a": False}, True))
        self.assertEqual(0.5, testNode.accessTable({"a": True}, False))
        self.assertEqual(0.092, testNode.accessTable({"a": True}, True))

        # test 2
        testNode = BayesianNode.BayesianNode("q", ["a", "b"])
        testNode.table = [#Pr(q=F | a, b), Pr(q=T | a, b)
                                (0.1,            0.2), # a:F, b:F
                                (0.3,            0.4), # a:F, b:T
                                (0.5,            0.6), # a:T, b:F
                                (0.7,            0.8)] # a:T, b:T
        self.assertEqual(0.1, testNode.accessTable({"a":False, "b": False}, False))
        self.assertEqual(0.2, testNode.accessTable({"a":False, "b": False}, True))
        self.assertEqual(0.3, testNode.accessTable({"a":False, "b": True}, False))
        self.assertEqual(0.4, testNode.accessTable({"a":False, "b": True}, True))
        self.assertEqual(0.5, testNode.accessTable({"a":True, "b": False}, False))
        self.assertEqual(0.6, testNode.accessTable({"a":True, "b": False}, True))
        self.assertEqual(0.7, testNode.accessTable({"a":True, "b": True}, False))
        self.assertEqual(0.8, testNode.accessTable({"a":True, "b": True}, True))

        # test many (5)
        testNode = BayesianNode.BayesianNode("q", ["a", "b", "c", "d", "e"])
        five_element_truth_table = [
            {"a": False, "b": False, "c":False, "d":False, "e":False},
            {"a": False, "b": False, "c":False, "d":False, "e":True},
            {"a": False, "b": False, "c":False, "d":True, "e":False},
            {"a": False, "b": False, "c":False, "d":True, "e":True},
            {"a": False, "b": False, "c":True, "d":False, "e":False},
            {"a": False, "b": False, "c":True, "d":False, "e":True},
            {"a": False, "b": False, "c":True, "d":True, "e":False},
            {"a": False, "b": False, "c":True, "d":True, "e":True},
            {"a": False, "b": True, "c":False, "d":False, "e":False},
            {"a": False, "b": True, "c":False, "d":False, "e":True},
            {"a": False, "b": True, "c":False, "d":True, "e":False},
            {"a": False, "b": True, "c":False, "d":True, "e":True},
            {"a": False, "b": True, "c":True, "d":False, "e":False},
            {"a": False, "b": True, "c":True, "d":False, "e":True},
            {"a": False, "b": True, "c":True, "d":True, "e":False},
            {"a": False, "b": True, "c":True, "d":True, "e":True},
            {"a": True, "b": False, "c":False, "d":False, "e":False},
            {"a": True, "b": False, "c":False, "d":False, "e":True},
            {"a": True, "b": False, "c":False, "d":True, "e":False},
            {"a": True, "b": False, "c":False, "d":True, "e":True},
            {"a": True, "b": False, "c":True, "d":False, "e":False},
            {"a": True, "b": False, "c":True, "d":False, "e":True},
            {"a": True, "b": False, "c":True, "d":True, "e":False},
            {"a": True, "b": False, "c":True, "d":True, "e":True},
            {"a": True, "b": True, "c":False, "d":False, "e":False},
            {"a": True, "b": True, "c":False, "d":False, "e":True},
            {"a": True, "b": True, "c":False, "d":True, "e":False},
            {"a": True, "b": True, "c":False, "d":True, "e":True},
            {"a": True, "b": True, "c":True, "d":False, "e":False},
            {"a": True, "b": True, "c":True, "d":False, "e":True},
            {"a": True, "b": True, "c":True, "d":True, "e":False},
            {"a": True, "b": True, "c":True, "d":True, "e":True}
        ]
        # creating table of Pr(q=F | a,b,c,d,e), Pr(q=T | a,b,c,d,e) where the values of a-e
        # are stored in five_element_truth_table
        five_element_truth_table_values = [
            ((2 * x / 1000), (2 * x + 1) / 1000) for x in range(len(five_element_truth_table))
        ]
        testNode.table = five_element_truth_table_values
        currentDict = None
        for x in range(len(five_element_truth_table)):
            currentDict = five_element_truth_table[x]
            self.assertEqual(five_element_truth_table_values[x],
                (testNode.accessTable(currentDict, False), testNode.accessTable(currentDict, True)))
'''

if __name__ == "__main__":
    rostest.rosrun("pyclassifiers", "BayesianNodeTest", BayesianNodeTest)
