#include <iostream>
#include "ConcurrentSystem.hpp"


int main()
{

    ConcurrentProcesser<uint64_t> fact("input.txt","output.txt", 5);
    try
    {
        fact.processFile();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}