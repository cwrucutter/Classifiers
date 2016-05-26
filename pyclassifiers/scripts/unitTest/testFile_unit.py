#!/usr/bin/env python

import unittest
import rospy
import rostest
from scripts.testFile import addValues

class addValueTest(unittest.TestCase):
    def test_add_0_0(self):
        self.assertEqual(0, addValues(0, 0))

    def test_add_1_3(self):
        self.assertEqual(4, addValues(1, 3))

if __name__ == "__main__":
    rostest.rosrun("pyclassifiers", "addValueTest", addValueTest)
