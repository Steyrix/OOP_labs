#include "../Headers/Factorizator.h"
#include "../Headers/MyException.h"
#include "../Headers/PollardRho.h"
#include <math.h>
#include <sstream>


using namespace std;


//Make queue for each thread and push numbers (readen from file) to available by space queues
//and process them in their threads.

Factorizator::Factorizator()
{
    Algorithm = algorithmUsed::NAIVE;
}

Factorizator::Factorizator(uint64_t num)
{

    Algorithm = algorithmUsed::POLLARD_RHO;
    factoriseNumber(num);
}


Factorizator::~Factorizator(){};

void Factorizator::Init(uint64_t num)
{
    factors.clear();
    factoriseNumber(num);
}

void Factorizator::factoriseNumber(uint64_t num)
{
    factors.clear();

    if(Algorithm == 0)
        factoriseByNaive(num);
    else factoriseByPollard(num);
}

void Factorizator::FactoriseToFile(const string &input, const string &output)
{
    ifstream fin(input);
    ofstream fout(output);

    if(!fin.good())
        throw IO_EXC("Unable to open input file!");

    if(!fout.good())
        throw IO_EXC("Unable to open/create output file!");

    uint64_t inNum;
    while(fin >> inNum)
    {
        factoriseNumber(inNum);
        fout << inNum << ": " << this->getFactors() << endl;
    }

    fin.close();
    fout.close();
}

void Factorizator::setAlgoritm(Factorizator::algorithmUsed algo)
{
    Algorithm = algo;
}


void Factorizator::factoriseByNaive(uint64_t num)
{
    while(num % 2 == 0)
    {
        factors.push_back(2);
        num /= 2;
    }
    uint64_t iMax = round(sqrt(num))+1;
    for(uint64_t i = 3; i <= iMax; i+=2)
    {
        while(num % i == 0)
        {
            factors.push_back(i);
            num /= i;
        }
    }

    if (num > 2)
        factors.push_back(num);
}

void Factorizator::factoriseByPollard(uint64_t num)
{
    srand(static_cast<int>(time(NULL)));

    if(isPrime(num))
    {
        factors.push_back(num);
        return;
    }

    while(!isPrime(num))
    {
        uint64_t pro = PollardRho(num);
        if(isPrime(pro))
        {
            factors.push_back(pro);
            num /= pro;
        }
        else
        {
            num /= pro;
            factoriseByPollard(pro);
        }
    }

    if(num != 1)
        factors.push_back(num);

}


string Factorizator::getFactors() const
{
    if(factors.empty())
        throw NO_FACTORS_EXC("Factorization was not done! Please, init factorizator with number.");

    ostringstream sout;

    copy(factors.begin(), factors.end()-1, ostream_iterator<uint64_t>(sout, " "));
    sout << factors.back();

    return sout.str();
}

uint64_t Factorizator::calculateNumByFactors() const
{
    if(factors.empty())
        throw NO_FACTORS_EXC("Factorization was not done! Please, init factorizator with number.");

    uint64_t out = 1;
    for(auto &it: factors)
        out *= it;

    return out;
}
