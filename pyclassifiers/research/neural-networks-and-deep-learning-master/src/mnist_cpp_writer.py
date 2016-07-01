# SYSTEM IMPORTS


# PYTHON PROJECT IMPORTS
import mnist_loader

class CustomWriter(object):
    def __init__(self, basePath, fileNames=[]):
        self.fileNames = fileNames
        self.basePath = basePath
        self.trainingData,self.valData,self.testData = \
            mnist_loader.load_data_wrapper()
        self.data = [self.trainingData, self.valData, self.testData]

        self.fileNameDataPairs = zip(self.fileNames, self.data)

    def writeHeader(self):
        with open(os.path.join(self.basePath, "mnist_data.hpp"), "w") as file:
            file.write(
                "#include <vector>\n"
                "#include <utility>\n\n"
                "namespace Classifiers\n"
                "{\n"
                "namespace Neural\n"
                "{\n"
                "namespace Tests\n"
                "{\n\n\n"

                "extern const std::vector<std::pair<std::vector<double>,\n"
                "                                   std::vector<double> > > trainingData;\n"
                "extern const std::vector<std::pair<std::vector<double>,\n"
                "                                   std::vector<double> > > valData;\n"
                "extern const std::vector<std::pair<std::vector<double>,\n"
                "                                   std::vector<double> > > testData;\n"


                "}\n"
                "}\n"
                "}\n"
            )

    def writeToFile(self, fileName, data):
        with open(os.path.join(self.basePath,
                               "mnist_" + fileName + ".cpp"), 'w') as file:
            file.write(
                "#include \"unitTest/data/mnist_data.hpp\"\n"
                "namespace Classifiers\n"
                "{\n"
                "namespace Neural\n"
                "{\n"
                "namespace Tests\n"
                "{\n\n\n"

                "   const std::vector<std::pair<std::vector<double>,\n"
                "                               std::vector<double> > >" + fileName + "=\\\n"
                "   {\n"
            )
            for x, y in data:
                file.write("\t\t{{")
                for element in xrange(0, len(x)):
                    file.write("%s" % x[element][0])
                    if element < len(x) - 1:
                        file.write(', ')
                file.write('}, {')
                if "trainingData" in fileName:
                    for element in xrange(0, len(y)):
                        file.write("%s" % y[element][0])
                        if element < len(y) - 1:
                            file.write(', ')
                else:
                    file.write("%s" % y)
                file.write('}},\n')
            file.write(
                "   \t};"
                "}\n"
                "}\n"
                "}\n"
            )

    def write(self):
        self.writeHeader()
        for fileName, data in self.fileNameDataPairs:
            self.writeToFile(fileName, data)


if __name__ == '__main__':
    import os
    basePath = os.path.dirname(os.path.abspath('.'))
    dataPath = os.path.join(basePath, "..", "..", "..",
                            "cppclassifiers", "cpp", "src",
                            "unitTest", "data")
    if not os.path.exists(dataPath):
        os.mkdir(dataPath)
    writer = CustomWriter(dataPath, ["trainingData",
                                     "valData",
                                     "testData"])
    writer.write()

