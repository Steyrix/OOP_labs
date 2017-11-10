#pragma once
#include "WavExceptions.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

using DataArray = std::vector<std::vector<short>>;

class WavCore
{
public:
    
    WavCore();
    ~WavCore();

    struct WavHeader
    {
        int8_t chunkId[4];
        uint32_t chunkSize;
        int8_t format[4];
        int8_t subchunk1Id[4];
        uint32_t subchunk1Size;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
        int8_t subchunk2Id[4];
        uint32_t subchunk2Size;
    };
    bool valid;
    WavHeader* getHeader();
    DataArray* getData();
    static const uint8_t chunkID[4];
    static const uint8_t format[4];
    static const uint8_t subchunk1Id[4];
    static const uint8_t subchunk2Id[4];
    static const uint8_t subchunk1Size;
    static const uint8_t audioFormat;
    static const uint8_t headerSize;
    static const uint8_t preChunkSize;
    static const uint8_t byteSize;
    void setHeader(const WavHeader& header);
    void setData(const DataArray& dataVector);
    void printInfo() const;
    void setFileSize(std::size_t fSize);
private:
    WavHeader header;
    DataArray channelsData;
    std::size_t fileSize;
    void SetHeaderZero();
    void SetDataZero();
    bool checkHeader() const;
    void checkRiff() const throw (HEADER_RIFF_EXC);
    void checkChunkSize() const throw (HEADER_FILE_SIZE_EXC);
    void checkWave() const throw (HEADER_WAVE_EXC);
    void checkFMT() const throw (HEADER_FMT_EXC);
    void checkPCM() const throw (HEADER_NOT_PCM_EXC);
    void checkSubchunk1() const throw (HEADER_SUBCHUNK1_EXC);
    void checkByteRate() const throw (HEADER_BYTERATE_EXC);
    void checkBlockAlign() const throw (HEADER_BLOCK_ALIGN_EXC);
    void checkSubchunk2() const throw (HEADER_SUBCHUNK2_SIZE_EXC);
    void checkAll() const;
};


