#pragma once
#include <exception>
#include <stdexcept>

using RE = std::runtime_error;

class NO_FACTORS_EXC : RE
{
public:
    explicit NO_FACTORS_EXC(const std::string &what_arg) : RE(what_arg){}
};

class IO_EXC : RE
{
public:
    explicit IO_EXC(const std::string &what_arg) : RE(what_arg){}
};