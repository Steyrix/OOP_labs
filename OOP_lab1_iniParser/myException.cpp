#include <stdexcept>
#include <stdio.h>
#include "myException.hpp"

InvalidSectionException::InvalidSectionException(){};
InvalidSectionException::~InvalidSectionException(){};
const char* InvalidSectionException::what() const throw()
{
    return "The chosen section does not exist.";
}

InvalidParameterException::InvalidParameterException(){};
InvalidParameterException::~InvalidParameterException(){};
const char* InvalidParameterException::what() const throw()
{
    return "The chosen parameter does not exist.";
}

iniNotParsedException::iniNotParsedException(){};
iniNotParsedException::~iniNotParsedException(){};
const char* iniNotParsedException::what() const throw()
{
    return "INI file is failed to be parsed or have not been chosen yet";
}

wrongFileException::wrongFileException(){};
wrongFileException::~wrongFileException(){};
const char* wrongFileException::what() const throw()
{
    return "Can't open the file! Check filename and format!";
}

InvalidDataTypeException::InvalidDataTypeException(){};
InvalidDataTypeException::~InvalidDataTypeException(){};
const char* InvalidDataTypeException::what() const throw()
{
    return "Parameter's value can't be parsed to chosen data type! Try another type";
}
