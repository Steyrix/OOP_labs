#include "../Headers/PollardRho.h"
#include <cstdlib>
#include <ctime>
#include <boost/multiprecision/cpp_int.hpp>

uint64_t myABS(int64_t num)
{
    return num >= 0 ? num : (num * -1);
}

uint64_t myGCD(uint64_t a, uint64_t b)
{
    return b ? myGCD(b, a % b) : a;
}

uint64_t modularPow(uint64_t base, uint64_t e, uint64_t modulo)
{
    if(e == 0)
        return 1;

    //Using unsigned int128 for multiplying big numbers
    boost::multiprecision::uint128_t result = 1;
    boost::multiprecision::uint128_t bs = base;

    while (e > 0)
    {
        if (e & 1)
            result = (result * bs) % modulo;

        e = e >> 1;

        bs = (bs * bs) % modulo;
    }
    return boost::numeric_cast<uint64_t>(result);
}

uint64_t PollardRho(uint64_t n)
{
    if(isPrime(n)) return n;
    if(n % 2 == 0) return 2;

    uint64_t x = (rand()%(n-2)) + 2;
    uint64_t y = x;

    uint64_t c = (rand()%(n-1)) + 1;
    uint64_t d = 1;

    while(d == 1)
    {
        //Slow increasing function aka 'Turtle move'
        x = (modularPow(x, 2, n) + c + n) % n;

        //Fast increasing function aka 'Hare move'
        y = (modularPow(y, 2, n) + c + n) % n;
        y = (modularPow(y, 2, n) + c + n) % n;

        d = myGCD(myABS(x-y), n);

        if(d == n && d < 1000000000)
            return PollardRho(n);
    }

    return d;
}

bool isPrime(uint64_t num)
{
    uint64_t a;

    if(num == 1 || num == 2 || num == 3 || num == 5 || num == 7 || num == 11)
        return true;

    if(num % 2 == 0 || num % 3 == 0 || num % 5 == 0 || num % 7 == 0 || num % 11 == 0)
        return false;

    for(uint16_t k = 0; k < 100; k++)
    {
        a = (rand() % (num-1)) + 1;
        if (modularPow(a, num - 1, num) != 1)
            return false;
    }
    return true;
}
