#!/usr/bin/env python

import unittest
import rospy
import rostest
import scripts.testFile as p
import sys
import os
from coverage import Coverage

class addValuesTest(unittest.TestCase):
    def test_add_0_0(self):
        self.assertEqual(0, p.addValues(0, 0))

    def test_add_1_3(self):
        self.assertEqual(4, p.addValues(1, 3))

if __name__ == "__main__":
    currentDir = os.path.dirname(os.path.realpath(__file__))
    cov = Coverage(source=os.path.join(currentDir, ".."))
    cov.start()
    rostest.rosrun("pyclassifiers", "addValueTest", addValuesTest)
    cov.stop()
    cov.xml_report(outfile=os.path.join(currentDir, "coverageTests.xml"))
