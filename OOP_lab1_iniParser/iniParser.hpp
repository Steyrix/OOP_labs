#ifndef iniParser_hpp
#define iniParser_hpp
#include <fstream>
#include <string>
#include <map>
#include "myException.hpp"

//Added for readability. Each section has its own map of parameters
typedef std::map<std::string, std::map<std::string,std::string>> DataMap;

class iniParser
{
public:
    iniParser();
    iniParser(const std::string &fName);
    ~iniParser();
    void initialize(const std::string &fName) throw (iniNotParsedException,);
    void show() const; //Print out all sections, parameters and their values
    void makeParameter(const std::string &sectionName, const std::string &paramName, const std::string &value) throw (iniNotParsedException);
    bool existSection(const std::string &sectionName) const throw (iniNotParsedException);
    bool existParameter(const std::string &sectionName, const std::string &parameterName) const throw
    (InvalidSectionException,iniNotParsedException);
    bool isParsed() const;
    template <typename T>
    T getValue (const std::string &sectionName, const std::string &paramName) const throw
    (InvalidSectionException, InvalidParameterException, InvalidDataTypeException);
private:
    DataMap dataMap;
    std::string fileName;
    std::ifstream fin;
    bool iniParsed;
    bool iniParse() throw (wrongFileException);
    void outputKeys() const;
    void outputSections() const;
    void findCommentInLine(const std::string &line, size_t &lineEnd) const;
    bool findSectionInLine(const std::string &line, std::string& lastSection) const;
    void findParamInLine(const std::string &line, const std::string &lastSection, size_t &lineEnd);
};

#endif
