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
import StateFunctor

class TestFunctor(StateFunctor.StateFunctor):
    def __init__(self):
        pass

    def hash(self, object):
        if not object:
            return 0
        else:
            return 1

    def __len__(self):
        return 2

    def getAllStates(self):
        return [False, True] 

class StateFunctorTest(unittest.TestCase):

    def test_inheritance(self):
        testF = TestFunctor()
        self.assertTrue(issubclass(TestFunctor, StateFunctor.StateFunctor))
        self.assertTrue(isinstance(testF, TestFunctor))

    def test_hash(self):
        testF = TestFunctor()
        self.assertEqual(0, testF.hash(False))
        self.assertEqual(1, testF.hash(True))

    def test___len__(self):
        testF = TestFunctor()
        self.assertEqual(2, len(testF))

    def test_getAllStates(self):
        testF = TestFunctor()
        self.assertEqual([False, True], testF.getAllStates())

    def test___str__(self):
        testF = TestFunctor()
        self.assertEqual("TestFunctor:\n\t[False, True]", "%s" % testF)

    def test___eq__(self):
        self.assertTrue(TestFunctor() == TestFunctor())

if __name__ == "__main__":
    rostest.rosrun("pyclassifiers", "StateFunctorTest", StateFunctorTest)

