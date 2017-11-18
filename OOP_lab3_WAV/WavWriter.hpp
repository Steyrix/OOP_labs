#pragma once
#include "WavCore.hpp"

class WavWriter
{
public:
    ~WavWriter();
    static void createSoundFile(const std::string &fileName, WavCore &soundFile);
private:
    static std::vector<short> makeData(int chanCount, size_t samplesCountPerChan, DataArray &data);
};
