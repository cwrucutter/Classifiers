
# SYSTEM IMPORTS
from abc import ABCMeta, abstractmethod

# PYTHON PROJECT IMPORTS


class StateFunctor(object):
    __metaclass__ = ABCMeta

    @abstractmethod
    def hash(self, object):
        pass

    @abstractmethod
    def __len__(self):
        pass

    @abstractmethod
    def getAllStates(self):
        pass

    def __str__(self):
        return "%s:\n\t%s" % (self.__class__.__name__, self.getAllStates())

    def __eq__(self, other):
        return (other is not None and isinstance(self, other.__class__)
            and isinstance(other, self.__class__))
