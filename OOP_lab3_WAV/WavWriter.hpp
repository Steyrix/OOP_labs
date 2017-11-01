#pragma once
#include "WavCore.hpp"

class WavWriter
{
public:
    explicit WavWriter(int bitsPerSample, int SampleRate, DataArray &data);
    WavWriter();
    ~WavWriter();
    void ReInit(int bitsPerSample, int sampleRate, DataArray &data);
    void ReInit(WavCore &WAV);
    void writeWAV(const std::string &fileName) const;
    
    static const uint8_t chunkID[4];
    static const uint8_t format[4];
    static const uint8_t subchunk1Id[4];
    static const uint8_t subchunk2Id[4];
    static const uint8_t subchunk1Size;
    static const uint8_t audioFormat;
    static const uint8_t headerSize;
    static const uint8_t preChunkSize;
    static const uint8_t byteSize;
private:
    bool maked;
    int bitsPerSampleWriter;
    WavCore::WavHeader header;
    std::vector<short> allChannels;
    bool makeWAVfile(int SampleRate, DataArray &data);
    bool rewriteWAVfile(WavCore &WAV);
    void writeOut(const std::string &fileName) const throw (IO_ERR_EXC, NULL_DATA);
    void prefillHeader();
    void prefillRIFF();
    void prefillWAVE();
    void prefillFMT();
    void prefillParams();
    void fillHeader(int chanCount, int sampleRate, size_t samplesCountPerChan) throw (UNSUPPORTED_FORMAT_EXC, BAD_PARAMS_EXC);
    void fillParams(int fileSizeBytes, int sampleRate, int chanCount);
    void makeData(int chanCount, size_t samplesCountPerChan, DataArray &data);
    
};
