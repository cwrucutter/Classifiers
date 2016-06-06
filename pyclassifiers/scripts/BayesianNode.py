
class BayesianNode(object):
    def __init__(self, name, dependentRV):
        self.name = name
        (self.dependentRV, self.table) = self.generateTable(dependentRV)

    def generateTable(self, dependentRV):
        return (None, None)
