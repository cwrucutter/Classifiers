
# SYSTEM IMPORTS

# PYTHON PROJECT IMPORTS
from BayesianNode import BayesianNode

class BayesianNetwork(object):

    def __init__(self, variables, relationships):
        assert isinstance(variables, list)
        assert isinstance(relationships, dict)
        self.variables = sorted(variables)
        self.nodes = {}
        for x in self.variables:
            assert (x in relationships and isinstance(relationships[x], list))
            self.nodes[x] = BayesianNode(x, relationships[x])

    # def 


