
# SYSTEM IMPORTS
from operator import itemgetter

# PYTHON PROJECT IMPORTS
from StateFunctor import StateFunctor


class BayesianNode(object):
    """
        A Bayesian Node is an object that stores the probability of a random variable
        conditioned upon a set of other random variables.
    """

    def __init__(self, name, nameStateFunctor, dependencies):
        """
            Constructs a BayesianNode object.

            Keyword arguments:
            name         -- the random variable this Bayesian Node is representing (string)
            dependencies -- a list of random variables that name is conditioned upon.
        """

        assert isinstance(name, str)
        assert issubclass(nameStateFunctor.__class__, StateFunctor)
        assert isinstance(dependencies, list)
        assert (len(dependencies) >= 0)

        self.name = name
        self.numRows = reduce(lambda rowSum, numDepStates: rowSum * numDepStates,
                              [len(tup[1]) for tup in dependencies], 1)
        self.nameStateFunctor = nameStateFunctor
        (self.dependencies, self.table) = self.generateTable(dependencies, nameStateFunctor)
        if len(self.table) > 1:
            self.totalProbabilities = (0 for x in range(len(self.nameStateFunctor)))
        if len(self.dependencies) > 0:
            self.parents = {x[0]: None for x in self.dependencies}
        self.children = None

    def generateTable(self, dependencies, nameStateFunctor):
        """
            Generates the empty table of condition probabilities. Should be called once
            by the constructor.

            Keyword arguments:
            dependencies -- a list of random variables that name is conditioned upon.
        """
        sortedDependencies = sorted(dependencies, key=lambda depName: depName[0])
        sortedDependencies = sorted(sortedDependencies, key=lambda numDepStates: len(numDepStates[1]), reverse=True)
        # generate empty table of (Pr(name=F | dependencies), Pr(name=T | dependencies)) tuples.
        # use bitwise lshift (left shift) operator for speed.
        # number of rows in the table = 2 ** <number_of_variables>
        truthTable = [[0 for i in range(len(nameStateFunctor))] for index in range(self.numRows)]
        return (sortedDependencies, truthTable)

    def valuesToIndex(self, depValues):
        """
            Converts an expression of dependencies into the index in the table.

            Keyword argument:
            depValues -- a dictionary of each dependency with state values.
        """

        # make this assertion in case valuesToIndex is called by something other
        # than self.accessTable()
        assert isinstance(depValues, dict)

        index = 0
        depStateFunctor = None
        depValue = None

        # the size of each "jump" in the truth table. Each variable spans a certain
        # number of rows in the truth table per column
        base = 1

        # run from least significant bit to most significant bit (backwards through list)
        # remember self.dependencies is sorted, so be consistent with this scheme.
        for x in range(len(self.dependencies) - 1, -1, -1):

            # need complete vector of conditional variables
            assert (self.dependencies[x][0] in depValues)

            # get the object to hash them and check that this object can hash them
            depStateFunctor = self.dependencies[x][1]
            assert issubclass(depStateFunctor.__class__, StateFunctor)

            # the given value of the conditional variable
            depValue = depValues[self.dependencies[x][0]]

            
            index += (depStateFunctor.hash(depValue) * base)

            # update the base. This is the product of the number of possible states
            # for each variable of every lesser significant bit in the vector.
            base *= len(self.dependencies[x][1])
        return index

    def accessTable(self, depValues, varValue):
        """
            Get the value from the table.

            Keyword arguments:
            depValues -- a dictionary of each dependency with state values.
            varValue  -- The state value of the variable this node is representing.
        """
        return self.table[self.valuesToIndex(depValues)]\
                [self.nameStateFunctor.hash(varValue)]

    def pr(self, state):
        """
            Calculate the probability that the variable this node is representing
            is a certain state.

            Keyword arguments:
            state -- the state that the variable of this node should be
        """
        if len(self.table) > 1:
            return self.totalProbabilities[self.nameStateFunctor.hash(state)]
        return self.table[0][self.nameStateFunctor.hash(state)]

    # def train(self, )
