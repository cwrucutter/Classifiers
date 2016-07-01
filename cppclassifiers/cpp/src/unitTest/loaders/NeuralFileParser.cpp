// SYSTEM INCLUDES
#include <fstream>

// C++ PROJECT INCLUDES
#include "unitTest/loaders/NeuralFileParser.hpp"

namespace Classifiers
{
namespace Neural
{
namespace Tests
{

    FileParser::FileParser() : _filePath(), _cachedData()
    {
    }

    FileParser::~FileParser()
    {
    }

    double FileParser::ParseDouble(std::string line, int& endIndex)
    {
        double result = 0.0;
        int placeCounter = 0, decimalCounter = 0, digit = 0;
        char c;

        while(endIndex >= 0 && endIndex != ' '
            && endIndex != '{' && endIndex != ',')
        {
            c = line[endIndex];

            if(c == '1') { digit = 1; }
            else if(c == '2') { digit = 2; }
            else if(c == '3') { digit = 3; }
            else if(c == '4') { digit = 4; }
            else if(c == '5') { digit = 5; }
            else if(c == '6') { digit = 6; }
            else if(c == '7') { digit = 7; }
            else if(c == '8') { digit = 8; }
            else if(c == '9') { digit = 9; }

            else if(c == '.')
            {
                if(decimalCounter == 1)
                {
                    // format error;
                }

                for(int j = 0; j < placeCounter; ++j)
                {
                    result /= 10.0;
                }

                placeCounter = -1;
                ++decimalCounter;
            }

            else if(c != '0')
            {
                // format error;
            }

            if(c != '0' && c != '.')
            {
                for(int j = 0; j < placeCounter; ++j)
                {
                    digit *= 10.0;
                }
                result += digit;
            }
            ++placeCounter;

            --endIndex;
        }
        return result;
    }

    std::vector<double> FileParser::ParseVector(std::string line, int& endIndex)
    {
        std::vector<double> vec;
        while(endIndex > 0 && line[endIndex] != '{')
        {
            if(line[endIndex] != '{' && line[endIndex] != '}'
                && line[endIndex] != ',' && line[endIndex] != ' ')
            {
                vec.push_back(this->ParseDouble(line, endIndex));
            }
            --endIndex;
        }
        return vec;
    }

    std::pair<std::vector<double>,
              std::vector<double> > FileParser::ReadLine(std::string line)
    {
        int endIndex = (int)line.size() - 1;
        std::vector<double> trainingInput;
        std::vector<double> trainingOutput;
        if(line[endIndex] == '}')
        {
            --endIndex;
        }
        trainingOutput = this->ParseVector(line, endIndex);
        while(endIndex > 0 && (line[endIndex] == ' ' || line[endIndex] == ','))
        {
            --endIndex;
        }
        trainingInput = this->ParseVector(line, endIndex);
        return std::make_pair(trainingInput, trainingOutput);
    }

    std::vector<std::pair<std::vector<double>,
                          std::vector<double> > > FileParser::ReadData(std::string filePath)
    {
        if(filePath == this->_filePath)
        {
            return this->_cachedData;
        }
        this->_filePath = filePath;
        std::ifstream file(filePath);
        std::vector<std::pair<std::vector<double>,
                              std::vector<double> > > readData;
        std::string line;
        if(file.is_open())
        {
            while(std::getline(file, line))
            {
                if(line.size() > 0)
                {
                    readData.push_back(this->ReadLine(line));
                }
            }
        }
        this->_cachedData = readData;
        return this->_cachedData;
    }

} // end of namespace Tests
} // end of namespace Neural
} // end of namespace Classifiers
