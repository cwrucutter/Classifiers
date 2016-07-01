#pragma once
#ifndef CLASSIFIERS_NEURAL_TESTS_FILEUTILS_HPP
#define CLASSIFIERS_NEURAL_TESTS_FILEUTILS_HPP

// SYSTEM INCLUDES
#include <string>
#include <utility>
#include <vector>

// C++ PROJECT INCLUDES


namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    class FileParser
    {
        public:

            FileParser();

            ~FileParser();

            double ParseDouble(std::string line, int& endIndex);

            std::vector<double> ParseVector(std::string line, int& endIndex);

            std::pair<std::vector<double>,
                      std::vector<double> > ReadLine(std::string line);

            std::vector<std::pair<std::vector<double>,
                                  std::vector<double> > > ReadData(std::string filePath);

        private:

            std::string _filePath;

            std::vector<std::pair<std::vector<double>,
                                  std::vector<double> > > _cachedData;

    };

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers

#endif // end of CLASSIFIERS_NEURAL_TESTS_FILEUTILS_HPP
