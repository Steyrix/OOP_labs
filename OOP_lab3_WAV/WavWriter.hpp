#pragma once
#include "WavCore.hpp"

class WavWriter
{
public:
    ~WavWriter();
    static void createSoundFile(const std::string &fileName, WavCore &soundFile) throw (IO_ERR_EXC, NULL_DATA, BAD_PARAMS_EXC);
private:
    static std::vector<short> makeData(int chanCount, size_t samplesCountPerChan, DataArray &data);
};
