#include "Factorizator.hpp"
#include "PollardRho.hpp"
#include <sstream>
#include <math.h>
using namespace std;

Factorizator::~Factorizator(){};

vector<uint64_t> Factorizator::factoriseByPollard(uint64_t num)
{
    vector<uint64_t> out;
    srand(static_cast<int>(time(NULL)));

    if(isPrime(num))
    {
        out.push_back(num);
        return out;
    }

    while(!isPrime(num))
    {
        uint64_t pro = PollardRho(num);
        if(isPrime(pro))
        {
            out.push_back(pro);
            num /= pro;
        }
        else
        {
            num /= pro;
            vector<uint64_t> ppro = factoriseByPollard(pro);
            out.insert(out.end(),ppro.begin(), ppro.end());
        }
    }

    if(num != 1)
        out.push_back(num);
    return out;
}

uint64_t Factorizator::calculateNumByFactors(const vector<uint64_t>& factors)
{
    if(factors.empty())
        throw std::runtime_error("Factorization was not done! Please, init factorizator with number.");

    uint64_t out = 1;
    for(auto &it: factors)
        out *= it;

    return out;
}

string Factorizator::getFactorStringFromVector(const vector<uint64_t>& factors)
{
    if(factors.empty())
        throw std::runtime_error("There are no factors to compute a number!");

    ostringstream oss;
    copy(factors.begin(), factors.end()-1, ostream_iterator<uint64_t>(oss, ","));
    oss << factors.back();
    return oss.str();
}

vector<uint64_t> Factorizator::factoriseByNaive(uint64_t num)
{
    vector<uint64_t> out;
    while(num % 2 == 0)
    {
        out.push_back(2);
        num /= 2;
    }
    uint64_t iMax = round(sqrt(num))+1;
    for(uint64_t i = 3; i <= iMax; i+=2)
    {
        while(num % i == 0)
        {
            out.push_back(i);
            num /= i;
        }
    }

    if (num > 2)
        out.push_back(num);

    return out;
}
