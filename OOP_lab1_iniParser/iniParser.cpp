#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "iniParser.hpp"
#include "myException.hpp"

using namespace std;

bool isInteger(const string &str)
{
    istringstream istr(str);
    int i;
    istr >> noskipws >> i;
    
    return istr.eof() && !istr.fail();
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

iniParser::iniParser(const string &fName)
{
    initialize(fName);
}

iniParser::~iniParser()
{
    dataMap.clear();
}

void iniParser::initialize(const string &fName)
{
    if(iniParsed)
        dataMap.clear();
    
    fileName = fName;
    
    iniParsed = iniParse();
}

bool iniParser::iniParse() throw (wrongFileException)
{
    string line;
    string lastSection;
    size_t lineEnd;
    ifstream fin;
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

void iniParser::findCommentInLine(const string &line, size_t& lineEnd) const
{
    size_t comment = line.find(';');
    if(comment != string::npos)
        lineEnd = comment;
    else lineEnd = line.length() - 1;
}

bool iniParser::findSectionInLine(const string &line, string& lastSection) const
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

void iniParser::findParamInLine(const string &line, const string &lastSection, size_t& lineEnd)
{
    size_t p3 = line.find('=');
    if(p3 != string::npos)
    {
        string parameter = line.substr(0, p3);
        string value = line.substr(p3 + 1, lineEnd - p3 - 1);
        
        dataMap[lastSection][parameter] = value;
    }
}

void iniParser::makeParameter(const string &sectionName, const string &paramName, const string &value)
{
    if(iniParsed)
        dataMap[sectionName][paramName] = value;
    else throw iniNotParsedException();
}

bool iniParser::existSection(const string &sectionName) const
{
    if(!iniParsed)
        throw iniNotParsedException();
    
    auto It = dataMap.find(sectionName);
    if (It != dataMap.end())
        return true;
    else
        return false;
}

bool iniParser::existParameter(const string &sectionName, const string &paramName) const
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
int iniParser::getValue(const string &sectionName, const string &paramName) const
{
    string res = getString(sectionName, paramName);
    if(isInteger(res))
        return stoi(res);
    else throw InvalidDataTypeException();
}

template <>
float iniParser::getValue(const string &sectionName, const string &paramName) const
{
    string res = getString(sectionName, paramName);
    if(isFloat(res))
        return stof(res);
    else throw InvalidDataTypeException();
}

template <>
string iniParser::getValue(const string &sectionName, const string &paramName) const
{
    return getString(sectionName, paramName);
}

string iniParser::getString(const string &sectionName, const string &paramName) const
{
    if(!iniParsed)
        throw iniNotParsedException();
    
    auto It = dataMap.find(sectionName);
    if(It != dataMap.end())
    {
        auto It_param = It->second.find(paramName);
        if(It_param != It->second.end())
            return It_param->second;
        else throw InvalidParameterException();
    }
    else throw InvalidSectionException();
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

bool iniParser::isParsed() const
{
    return iniParsed;
}
