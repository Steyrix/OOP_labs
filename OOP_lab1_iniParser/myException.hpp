#ifndef myException_hpp
#define myException_hpp
#include <exception>

class InvalidSectionException : std::exception
{
public:
    InvalidSectionException();
    ~InvalidSectionException();
    const char* what() const throw() override;
};

class InvalidParameterException : std::exception
{
public:
    InvalidParameterException();
    ~InvalidParameterException();
    const char* what() const throw() override;
};

class iniNotParsedException : std::exception
{
public:
    iniNotParsedException();
    ~iniNotParsedException();
    const char* what() const throw() override;
};

class wrongFileException : std::exception
{
public:
    wrongFileException();
    ~wrongFileException();
    const char* what() const throw() override;
};

class InvalidDataTypeException : std::exception
{
public:
    InvalidDataTypeException();
    ~InvalidDataTypeException();
    const char* what() const throw() override;
};

#endif /* myException_hpp */
