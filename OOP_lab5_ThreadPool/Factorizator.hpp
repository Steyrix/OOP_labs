#pragma once
#include <string>
#include <vector>

class Factorizator
{
public:
    ~Factorizator();
    static std::vector<uint64_t> factoriseByPollard(uint64_t num);
    static std::vector<uint64_t> factoriseByNaive(uint64_t num);
    static uint64_t calculateNumByFactors(const std::vector<uint64_t>& factors);
    static std::string getFactorStringFromVector(const std::vector<uint64_t>& factors);
};
