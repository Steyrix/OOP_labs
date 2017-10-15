#include "myException.hpp"

const char* CopyingFromItself::what() const throw ()
{
    return "Adding data to object from itself is not allowed!";
}
