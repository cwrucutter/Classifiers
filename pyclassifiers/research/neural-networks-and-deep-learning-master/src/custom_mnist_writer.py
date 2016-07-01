# SYSTEM IMPORTS


# PYTHON PROJECT IMPORTS
import mnist_loader

class CustomWriter(object):
    def __init__(self, fileNames=[]):
        self.fileNames = fileNames
        self.trainingData,self.valData,self.testData = \
            mnist_loader.load_data_wrapper()
        self.data = [self.trainingData, self.valData, self.testData]

        self.fileNameDataPairs = zip(self.fileNames, self.data)

    def writeToFile(self, fileName, data):
        with open(fileName, 'w') as file:
            for x, y in data:
                file.write("{{")
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
                file.write('}}\n')

    def write(self):
        for fileName, data in self.fileNameDataPairs:
            self.writeToFile(fileName, data)


if __name__ == '__main__':
    import os
    basePath = os.path.dirname(os.path.abspath('.'))
    dataPath = os.path.join(basePath, "data")
    writer = CustomWriter([os.path.join(dataPath, "trainingData.txt"),
                           os.path.join(dataPath, "validationData.txt"),
                           os.path.join(dataPath, "testData.txt")])
    writer.write()

