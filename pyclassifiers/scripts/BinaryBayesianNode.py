
class BinaryBayesianNode(object):
    """
        A Bayesian Node is an object that stores the probability of a random variable
        conditioned upon a set of other random variables.
    """

    def __init__(self, name, dependencies):
        """
            Constructs a BayesianNode object.

            Keyword arguments:
            name         -- the random variable this Bayesian Node is representing (string)
            dependencies -- a list of random variables that name is conditioned upon.
        """

        assert isinstance(name, str)
        assert isinstance(dependencies, list)
        assert (len(dependencies) >= 0)

        self.name = name
        (self.dependencies, self.table) = self.generateTable(dependencies)

    def generateTable(self, dependencies):
        """
            Generates the empty table of condition probabilities. Should be called once
            by the constructor.

            Keyword arguments:
            dependencies -- a list of random variables that name is conditioned upon.
        """

        sortedDependencies = sorted(dependencies)
        # generate empty table of (Pr(name=F | dependencies), Pr(name=T | dependencies)) tuples.
        # use bitwise lshift (left shift) operator for speed.
        # number of rows in the table = 2 ** <number_of_variables>
        truthTable = [(0, 0) for index in range(1 << len(sortedDependencies))]
        return (sortedDependencies, truthTable)

    def valuesToIndex(self, depValues):
        """
            Converts a binary expression of dependencies into the index in the table.

            Keyword argument:
            depValues -- a dictionary of each dependency with True | False values.
        """

        # make this assertion in case valuesToIndex is called by something other
        # than self.accessTable()
        assert isinstance(depValues, dict)

        # going to check that each variable is given a value and convert
        # the binary expression into an integer at the same time. This iterative
        # approach cuts time by using small bit shifts.
        index = 0
        base = 1

        # run from least significant bit to most significant bit (backwards through list)
        # remember self.dependencies is sorted, so be consistent with this scheme.
        for x in range(len(self.dependencies) - 1, -1, -1):
            assert (self.dependencies[x] in depValues)
            # if(true) is faster than multiplication (assuming small jump distance in code)
            if depValues[self.dependencies[x]]:
                index += base
            # bit shift for speed....x << y returns x shifted by y places...same as x * (2 ** y)
            base = base << 1
        return index

    def accessTable(self, depValues, varValue):
        """
            Get the value from the table.

            Keyword arguments:
            depValues -- a dictionary of each dependency with True | False values.
            varValue  -- True | False value of the variable this node is representing.
        """

        assert (varValue or not varValue)
        assert isinstance(depValues, dict)
        conditionalProb = self.table[self.valuesToIndex(depValues)][varValue]
        return conditionalProb

    # def train(self, )
