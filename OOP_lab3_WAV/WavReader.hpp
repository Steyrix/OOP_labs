#pragma once
#include "WavCore.hpp"

class WavReader
{
public:
    explicit WavReader(const std::string &fileName);
    ~WavReader();
    void ReInit(const std::string &fileName);
    WavCore* getWAV();
    static const uint8_t headerSize;
private:
    DataArray dataToSet;
    WavCore WAV;
    std::size_t fileSize;
    bool readHeader(const std::string &fileName) throw (IO_ERR_EXC, BAD_FORMAT_EXC);
    bool readData(const std::string &fileName) throw (IO_ERR_EXC);
    bool extractWavData(const std::string &fileName) throw (IO_ERR_EXC, UNSUPPORTED_FORMAT_EXC);
    std::streampos getBytesReadCount(std::streampos pos, std::ifstream &stream);
    void separateChannelsData(const std::vector<short>& allChannels, int chanCount, int samplesPerChan);
    void setTempZero(WavCore::WavHeader &headerTemp);
};
