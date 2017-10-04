#include <stdio.h>
#include <iostream>
#include <sstream>
#include "iniParser.hpp"
#include "myException.hpp"

using namespace std;

bool isInteger(const string &str)
{
    if(str.empty() || (!(isdigit(str[0]) &&
                    (str[0] != '-') && (str[0] != '+'))))
        return false;
     
    char *c;
    strtol(str.c_str(), &c, 10);
    return (*c == 0);
}

bool isFloat(const string &str)
{
    istringstream istr(str);
    float f;
    istr >> noskipws >> f;
    
    return istr.eof() && !istr.fail();
}

iniParser::iniParser()
{
    iniParsed = false;
}

iniParser::iniParser(string const &fName)
{
    fileName = fName;
    
    bool result = iniParse();
    if(result)
        iniParsed = true;
    else
    {
        iniParsed = false;
        throw iniNotParsedException();
    }
}

iniParser::~iniParser()
{
    dataMap.clear();
}

bool iniParser::initialize(string const &fName) throw (iniNotParsedException)
{
    if(iniParsed)
        dataMap.clear();
    
    fileName = fName;
    
    bool result = iniParse();
    iniParsed = result;
    if(result)
        return true;
    else
        throw iniNotParsedException();
}

bool iniParser::iniParse() throw (wrongFileException)
{
    string line;
    string lastSection;
    size_t lineEnd;
    
    fin.open(fileName.c_str());
    if(fin.is_open())
    {
        while(getline(fin, line))
        {
            if(line.empty())
                continue;
            
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            
            if(line[0] == ';')
                continue;
            
            lineEnd = line.length()-1;
            
            findCommentInLine(line, lineEnd);
            
            if(!findSectionInLine(line, lastSection))
                findParamInLine(line, lastSection, lineEnd);
        }
        
        fin.close();
        return true;
    }
    else
    {
        throw wrongFileException();
        return false;
    }
}

void iniParser::findCommentInLine(string const &line, size_t& lineEnd) const
{
    size_t comment = line.find(';');
    if(comment != string::npos)
        lineEnd = comment;
    else lineEnd = line.length() - 1;
}

bool iniParser::findSectionInLine(string const &line, string& lastSection) const
{
    size_t p1 = line.find('[');
    size_t p2 = line.find(']');
    
    if(p1 != string::npos && p2 != string::npos)
    {
        
        lastSection = line.substr(p1+1, p2 - p1 - 1);
        return true;
    }
    
    return false;
}

void iniParser::findParamInLine(string const &line, string const &lastSection, size_t& lineEnd)
{
    size_t p3 = line.find('=');
    if(p3 != string::npos)
    {
        string parameter = line.substr(0, p3);
        string value = line.substr(p3 + 1, lineEnd - p3 - 1);
        
        dataMap[lastSection][parameter] = value;
    }
}

void iniParser::makeParameter(string const &sectionName, string const &paramName, string const &value) throw(iniNotParsedException)
{
    if(iniParsed)
        dataMap[sectionName][paramName] = value;
    else throw iniNotParsedException();
}

bool iniParser::existSection(string const &sectionName) const throw (iniNotParsedException)
{
    if(!iniParsed)
        throw iniNotParsedException();
    
    auto It = dataMap.find(sectionName);
    if (It != dataMap.end())
        return true;
    else
        return false;
}

bool iniParser::existParameter(string const &sectionName, string const &paramName) const throw (InvalidSectionException, iniNotParsedException)
{
    if(!iniParsed)
        throw iniNotParsedException();
    
    auto It = dataMap.find(sectionName);
    if (It != dataMap.end())
    {
        auto It_param = It->second.find(paramName);
        if(It_param != It->second.end())
            return true;
        else return false;
    }
    else throw InvalidSectionException();
}

template <>
int iniParser::getValue(string const &sectionName, string const &paramName) const throw
(InvalidSectionException, InvalidParameterException, InvalidDataTypeException)
{
    if(!iniParsed)
        throw iniNotParsedException();
    
    auto It = dataMap.find(sectionName);
    if (It != dataMap.end())
    {
        auto It_param = It->second.find(paramName);
        if(It_param != It->second.end())
        {
            if(isInteger(It_param->second))
                return stoi(It_param->second);
            else throw InvalidDataTypeException();
        }
        else
            throw InvalidParameterException();
    }
    else
        throw InvalidSectionException();
    
}

template <>
float iniParser::getValue(string const &sectionName, string const &paramName) const throw
(InvalidSectionException, InvalidParameterException, InvalidDataTypeException)
{
    if(!iniParsed)
        throw iniNotParsedException();
    
    auto It = dataMap.find(sectionName);
    if (It != dataMap.end())
    {
        auto It_param = It->second.find(paramName);
        if(It_param != It->second.end())
        {
            if(isFloat(It_param->second))
                return stof(It_param->second);
            else throw InvalidDataTypeException();
        }
        else
            throw InvalidParameterException();
    }
    else
        throw InvalidSectionException();
    
}

template <>
string iniParser::getValue(string const &sectionName, string const &paramName) const throw
(InvalidSectionException, InvalidParameterException, InvalidDataTypeException)
{
    if(!iniParsed)
        throw iniNotParsedException();
    
    auto It = dataMap.find(sectionName);
    if (It != dataMap.end())
    {
        auto It_param = It->second.find(paramName);
        if(It_param != It->second.end())
            return It_param->second;
        else
            throw InvalidParameterException();
    }
    else
        throw InvalidSectionException();
}

void iniParser::outputKeys() const
{
    std::cout << "==INFO:== " << std::endl;
    for(auto it = dataMap.begin(); it != dataMap.end(); it++)
    {
        std::cout << it->first << std::endl;
        for(auto p = it->second.begin(); p!= it->second.end();p++)
        {
            std::cout << p->first << " = " << p->second << std::endl;
        }
    }
}

void iniParser::outputSections() const
{
    std::cout << "==SECTIONS:== " << std::endl;
    for(auto it = dataMap.begin(); it != dataMap.end(); it++)
    {
        std::cout << it->first << std::endl;
    }
}

void iniParser::show() const
{
    if(!iniParsed) return;
    outputKeys();
    outputSections();
}
