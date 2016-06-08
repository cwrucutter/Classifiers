
# SYSTEM IMPORTS
import sys
import os



currentDir = os.path.dirname(os.path.realpath(__file__))
scriptsDir = os.path.abspath(os.path.join(currentDir, "..", ".."))
sys.path.extend([currentDir, scriptsDir])


# PYTHON PROJECT IMPORTS
from pyclassifiers import StateFunctor

class BooleanFunctor(StateFunctor):
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

class HairColorFunctor(StateFunctor):
    def __init__(self):
        pass

    def hash(self, object):
        if object == "brown":
            return 0
        elif object == "blonde":
            return 1
        elif object == "black":
            return 2

    def __len__(self):
        return 3

    def getAllStates(self):
        return ["brown", "blonde", "black"]

class EyeColorFunctor(StateFunctor):
    def __init__(self):
        pass

    def hash(self, object):
        if object == "brown":
            return 0
        elif object == "blue":
            return 1
        elif object == "black":
            return 2
        elif object == "green":
            return 3

    def __len__(self):
        return 4

    def getAllStates(self):
        return ["brown", "blue", "black", "green"]
