#pragma once
#include "PollardRho.hpp"
#include <string>
#include <vector>
#include <fstream>

class Factorizator
{
public:
    enum algorithmUsed
    {
        NAIVE = 0,
        POLLARD_RHO = 1
    };
    explicit Factorizator(uint64_t num);
    Factorizator();
    ~Factorizator();
    void Init(uint64_t num);
    void FactoriseToFile(const std::string& input, const std::string& output);
    void setAlgoritm(algorithmUsed algo);
    std::string getFactors() const;
    uint64_t calculateNumByFactors() const;

private:
    algorithmUsed Algorithm;
    std::vector<uint64_t> factors;
    void factoriseNumber(uint64_t num);
    void factoriseByNaive(uint64_t num);
    void factoriseByPollard(uint64_t num);
};
