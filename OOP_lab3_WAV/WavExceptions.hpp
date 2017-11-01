#pragma once
#include <exception>
#include <stdexcept>

using RE = std::runtime_error;

class IO_ERR_EXC : RE
{
public:
    explicit IO_ERR_EXC(const std::string &what_arg) : RE(what_arg){}
};

class BAD_FORMAT_EXC : RE
{
public:
    explicit BAD_FORMAT_EXC(const std::string &what_arg) : RE(what_arg){}
};

class UNSUPPORTED_FORMAT_EXC : RE
{
public:
    explicit UNSUPPORTED_FORMAT_EXC(const std::string &what_arg) : RE(what_arg){}
};

class BAD_PARAMS_EXC : RE
{
public:
    explicit BAD_PARAMS_EXC(const std::string &what_arg) : RE(what_arg){}
};

class DATA_SIZE_EXC : RE
{
public:
    explicit DATA_SIZE_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_NOT_PARSED_EXC : RE
{
public:
    explicit HEADER_NOT_PARSED_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_RIFF_EXC : RE
{
public:
    explicit HEADER_RIFF_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_FILE_SIZE_EXC : RE
{
public:
    explicit HEADER_FILE_SIZE_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_WAVE_EXC : RE
{
public:
    explicit HEADER_WAVE_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_FMT_EXC : RE
{
public:
    explicit HEADER_FMT_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_NOT_PCM_EXC : RE
{
public:
    explicit HEADER_NOT_PCM_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_SUBCHUNK1_EXC : RE
{
public:
    explicit HEADER_SUBCHUNK1_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_BYTERATE_EXC : RE
{
public:
    explicit HEADER_BYTERATE_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_BLOCK_ALIGN_EXC : RE
{
public:
    explicit HEADER_BLOCK_ALIGN_EXC(const std::string &what_arg) : RE(what_arg){}
};

class HEADER_SUBCHUNK2_SIZE_EXC : RE
{
public:
    explicit HEADER_SUBCHUNK2_SIZE_EXC(const std::string &what_arg) : RE(what_arg){}
};

class NULL_DATA : RE
{
public:
    explicit NULL_DATA(const std::string &what_arg) : RE(what_arg){}
};







