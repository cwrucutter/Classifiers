
class BayesianNode(object):
    def __init__(self, name, dependencies):
        assert isinstance(name, str)
        assert isinstance(dependencies, list)
        assert (len(dependencies) > 0)

        self.name = name
        (self.dependencies, self.table) = self.generateTable(dependencies)

    def generateTable(self, dependencies):
        sortedDependencies = sorted(dependencies)
        truthTable = [(0, 0) for index in range(2 ** len(sortedDependencies))]
        return (sortedDependencies, truthTable)

    def valuesToIndex(self, depValues):
        assert isinstance(depValues, dict)
        index = 0
        base = 1
        for x in range(len(self.dependencies) - 1, -1, -1):
            assert (self.dependencies[x] in depValues)
            if depValues[self.dependencies[x]]:
                index += base
            base *= 2
        return index

    def accessTable(self, depValues, varValue):
        assert (varValue or not varValue)
        assert isinstance(depValues, dict)
        conditionalProb = self.table[self.valuesToIndex(depValues)][varValue]
        return conditionalProb
