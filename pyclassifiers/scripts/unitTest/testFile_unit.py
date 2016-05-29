#!/usr/bin/env python

# SYSTEM IMPORTS
import unittest
import rospy
import rostest
import sys
import os
from coverage import Coverage



currentDir = os.path.dirname(os.path.realpath(__file__))
scriptsDir = os.path.abspath(os.path.join(currentDir, "..", ".."))
sys.path.extend([currentDir, scriptsDir])

print("sys.path: %s" % sys.path)

# PYTHON PROJECT IMPORTS
import pyclassifiers

class addValuesTest(unittest.TestCase):
    def test_add_0_0(self):
        self.assertEqual(0, pyclassifiers.addValues(0, 0))

    def test_add_1_3(self):
        self.assertEqual(4, pyclassifiers.addValues(1, 3))

if __name__ == "__main__":
    # currentDir = os.path.dirname(os.path.realpath(__file__))
    cov = Coverage(source=[scriptsDir])
    cov.start()
    rostest.rosrun("pyclassifiers", "addValueTest", addValuesTest)
    cov.stop()
    cov.xml_report(outfile=os.path.join(currentDir, "coverageTests.xml"))
