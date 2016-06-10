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
import BayesianNetwork

'''
class BayesianNetworkTest(unittest.TestCase):

    def test_constructor(self):
        # test 0 input

        # test 2 input
        testNet = BayesianNetwork.BayesianNetwork(["a", "b"], {"a": ["b"], "b": []})
        self.assertEqual(["a", "b"], testNet.variables)
        self.assertEqual(2, len(testNet.nodes))

        # test many input
'''

if __name__ == "__main__":
    rostest.rosrun("pyclassifiers", "BayesianNetworkTest", BayesianNetworkTest)
