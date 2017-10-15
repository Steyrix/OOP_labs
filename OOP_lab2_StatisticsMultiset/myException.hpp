#ifndef myException_hpp
#define myException_hpp

#include <exception>

class CopyingFromItself : std::exception
{
public:
    const char* what() const throw () override;
};


#endif /* myException_hpp */
