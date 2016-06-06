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

print("sys.path: %s" % sys.path)

# PYTHON PROJECT IMPORTS
import pyclassifiers

class BayesianNodeTest(unittest.TestCase):
    def test_constructor(self):
        testNode = pyclassifiers.BayesianNode("testName", [])
        self.assertEqual("testName", testNode.name)
        self.assertEqual(None, testNode.dependentRV)
        self.assertEqual(None, testNode.table)

if __name__ == "__main__":
    rostest.rosrun("pyclassifiers", "BayesianNodeTest", BayesianNodeTest)
