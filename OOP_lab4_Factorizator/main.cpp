#include <iostream>
#include "Headers/Factorizator.h"


using namespace std;

int main()
{
    //Factorizator fnaive;
    //fnaive.Init(4765955355892171345);
    //cout << "Naive algorithm: \n" << fnaive.getFactors() << endl;

    Factorizator fpro(8233234222222222222);
    cout << "\n Pollard-rho algorithm: \n" << fpro.getFactors() << endl;
    cout << fpro.calculateNumByFactors() << endl;

    fpro.FactoriseToFile("input.txt", "output.txt");

    return 0;
}
