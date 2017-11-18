#include <iostream>
#include "Factorizator.hpp"

using namespace std;

int main()
{
    Factorizator fnaive;
    fnaive.Init(4765955355892171345);
    cout << "Naive algorithm: \n" << fnaive.getFactors() << endl;
    
    Factorizator fpro(4765955355892171345);
    cout << "Pollard-rho algorithm: \n" << fpro.getFactors() << endl;
    cout << fpro.calculateNumByFactors() << endl;

    fpro.FactoriseToFile("input.txt", "output.txt");
    
    return 0;
}
