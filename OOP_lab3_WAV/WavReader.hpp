#pragma once
#include "WavCore.hpp"

class WavReader
{
public:
    ~WavReader();
    static const uint8_t headerSize;
    static WavCore readSoundFile (const std::string &fileName);
private:
    static void readHeader(const std::string &fileName, WavCore &destinationFile);
    static void readData(const std::string &fileName, WavCore &destinationFile);
    static void separateChannelsData(const std::vector<short>& allChannels, DataArray &dataToSet, int chanCount, int samplesPerChan);
    static void setTempZero(WavCore::WavHeader &headerTemp);
    static std::streampos getBytesReadCount(std::streampos pos, std::ifstream &stream);
};
