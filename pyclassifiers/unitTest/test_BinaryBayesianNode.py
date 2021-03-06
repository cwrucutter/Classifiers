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
import BinaryBayesianNode

class BinaryBayesianNodeTest(unittest.TestCase):

    def test_constructor(self):
        # test 0 dependencies
        testNode = BinaryBayesianNode.BinaryBayesianNode("testName", [])
        self.assertEqual("testName", testNode.name)
        self.assertEqual(0, len(testNode.dependencies))
        self.assertEqual([], testNode.dependencies)
        self.assertEqual(1, len(testNode.table))
        self.assertEqual([(0, 0)], testNode.table)

        # test 1 input
        testNode = BinaryBayesianNode.BinaryBayesianNode("testName", ["a"])
        self.assertEqual("testName", testNode.name)
        self.assertEqual(1, len(testNode.dependencies))
        self.assertEqual(["a"], testNode.dependencies)
        self.assertEqual(2, len(testNode.table))
        self.assertEqual([(0, 0), (0, 0)], testNode.table)

        # test 2 input
        testNode = BinaryBayesianNode.BinaryBayesianNode("testName", ["b", "a"])
        self.assertEqual("testName", testNode.name)
        self.assertEqual(2, len(testNode.dependencies))
        self.assertEqual(["a", "b"], testNode.dependencies)
        self.assertEqual(4, len(testNode.table))
        self.assertEqual([(0, 0), (0, 0), (0, 0), (0, 0)], testNode.table)

        # test many (5)
        testNode = BinaryBayesianNode.BinaryBayesianNode("testName", ["a", "b", "e", "d", "c"])
        self.assertEqual("testName", testNode.name)
        self.assertEqual(5, len(testNode.dependencies))
        self.assertEqual(["a", "b", "c", "d", "e"], testNode.dependencies)
        self.assertEqual(32, len(testNode.table))
        self.assertEqual([(0, 0) for x in range(32)], testNode.table)

    def test_valuesToIndex(self):
        # test 0 input
        testNode = BinaryBayesianNode.BinaryBayesianNode("testName", [])
        self.assertEqual(0, testNode.valuesToIndex({}))

        # test 1
        testNode = BinaryBayesianNode.BinaryBayesianNode("testName", ["a"])
        self.assertEqual(0, testNode.valuesToIndex({"a": False}))
        self.assertEqual(1, testNode.valuesToIndex({"a": True}))

        # test 2
        testNode = BinaryBayesianNode.BinaryBayesianNode("testName", ["a", "b"])
        self.assertEqual(0, testNode.valuesToIndex({"a": False, "b": False}))
        self.assertEqual(1, testNode.valuesToIndex({"a": False, "b": True}))
        self.assertEqual(2, testNode.valuesToIndex({"a": True, "b": False}))
        self.assertEqual(3, testNode.valuesToIndex({"a": True, "b": True}))

        # test many (5)
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
        testNode = BinaryBayesianNode.BinaryBayesianNode("testName", ["a", "b", "e", "d", "c"])
        for x in range(len(five_element_truth_table)):
            self.assertEqual(x, testNode.valuesToIndex(five_element_truth_table[x]))
        

    def test_accessTable(self):
        # test 0
        testNode = BinaryBayesianNode.BinaryBayesianNode("q", [])
        testNode.table = [#Pr(q=F), Pr(q=T)
                            (0.7,     0.3)]
        self.assertEqual(0.7, testNode.accessTable({}, False))
        self.assertEqual(0.3, testNode.accessTable({}, True))

        # test 1
        testNode = BinaryBayesianNode.BinaryBayesianNode("q", ["a"])
        testNode.table = [# Pr(q=F | a), Pr(q=T | a)
                                 (0.01,        0.000234), # a:F
                                 (0.5,         0.092)]    # a:T
        self.assertEqual(0.01, testNode.accessTable({"a": False}, False))
        self.assertEqual(0.000234, testNode.accessTable({"a": False}, True))
        self.assertEqual(0.5, testNode.accessTable({"a": True}, False))
        self.assertEqual(0.092, testNode.accessTable({"a": True}, True))

        # test 2
        testNode = BinaryBayesianNode.BinaryBayesianNode("q", ["a", "b"])
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
        testNode = BinaryBayesianNode.BinaryBayesianNode("q", ["a", "b", "c", "d", "e"])
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


if __name__ == "__main__":
    rostest.rosrun("pyclassifiers", "BayesianNodeTest", BinaryBayesianNodeTest)
