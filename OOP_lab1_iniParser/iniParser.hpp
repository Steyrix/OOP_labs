#ifndef iniParser_hpp
#define iniParser_hpp
#include <fstream>
#include <string>
#include <map>
#include "myException.hpp"

//Added for readability. Each section has it own map of parameters
typedef std::map<std::string, std::map<std::string,std::string>> DataMap;

class iniParser
{
private:
    std::string fileName;
    std::ifstream fin;
    DataMap dataMap;
    bool iniParsed;
    bool iniParse() throw (wrongFileException);
    void outputKeys() const;
    void outputSections() const;
    void findCommentInLine(std::string const &line, size_t &lineEnd) const;
    bool findSectionInLine(std::string const &line, std::string& lastSection) const;
    void findParamInLine(std::string const &line, std::string const &lastSection, size_t &lineEnd);
public:
    iniParser();
    iniParser(std::string const &fName);
    ~iniParser();
    bool initialize(std::string const &fName) throw (iniNotParsedException,);
    void show() const; //Print out all sections, parameters and their values
    void makeParameter(std::string const &sectionName, std::string const &paramName, std::string const &value) throw (iniNotParsedException);
    bool existSection(std::string const &sectionName) const throw (iniNotParsedException);
    bool existParameter(std::string const &sectionName, std::string const &parameterName) const throw
    (InvalidSectionException,iniNotParsedException);
    template <typename T>
    T getValue (std::string const &sectionName, std::string const &paramName) const throw
    (InvalidSectionException, InvalidParameterException, InvalidDataTypeException);
};

#endif
