#include "WavWriter.hpp"
#include <iostream>

using namespace std;

const uint8_t WavWriter::chunkID[4] = {'R','I','F','F'};
const uint8_t WavWriter::format[4] = {'W','A','V','E'};
const uint8_t WavWriter::subchunk1Id[4] = {'f','m','t',' '};
const uint8_t WavWriter::subchunk2Id[4] = {'d','a','t','a'};
const uint8_t WavWriter::subchunk1Size = 16;
const uint8_t WavWriter::audioFormat = 1;
const uint8_t WavWriter::headerSize = 44;
const uint8_t WavWriter::preChunkSize = 8;
const uint8_t WavWriter::byteSize = 8;

WavWriter::WavWriter()
{}

WavWriter::WavWriter(int bitsPerSample, int sampleRate, DataArray &data)
{
    bitsPerSampleWriter = bitsPerSample;
    maked = makeWAVfile(sampleRate, data);
}

void WavWriter::ReInit(int bitsPerSample, int sampleRate, DataArray &data)
{
    bitsPerSampleWriter = bitsPerSample;
    allChannels.clear();
    maked = makeWAVfile(sampleRate, data);
}

void WavWriter::ReInit(WavCore &WAV)
{
    allChannels.clear();
    maked = rewriteWAVfile(WAV);
}

WavWriter::~WavWriter(){}

void WavWriter::prefillHeader()
{
    prefillRIFF();
    prefillWAVE();
    prefillFMT();
    prefillParams();
}

void WavWriter::prefillRIFF()
{
    header.chunkId[0] = chunkID[0];
    header.chunkId[1] = chunkID[1];
    header.chunkId[2] = chunkID[2];
    header.chunkId[3] = chunkID[3];
}

void WavWriter::prefillWAVE()
{
    header.format[0] = format[0];
    header.format[1] = format[1];
    header.format[2] = format[2];
    header.format[3] = format[3];
}

void WavWriter::prefillFMT()
{
    header.subchunk1Id[0] = subchunk1Id[0];
    header.subchunk1Id[1] = subchunk1Id[1];
    header.subchunk1Id[2] = subchunk1Id[2];
    header.subchunk1Id[3] = subchunk1Id[3];
    
    header.subchunk2Id[0] = subchunk2Id[0];
    header.subchunk2Id[1] = subchunk2Id[1];
    header.subchunk2Id[2] = subchunk2Id[2];
    header.subchunk2Id[3] = subchunk2Id[3];
}

void WavWriter::prefillParams()
{
    header.audioFormat   = 1;
    header.subchunk1Size = 16;
    header.bitsPerSample = bitsPerSampleWriter;
}

void WavWriter::fillHeader(int chanCount, int sampleRate, size_t samplesCountPerChan) throw (UNSUPPORTED_FORMAT_EXC, BAD_PARAMS_EXC)
{
    if(chanCount < 1)
        throw BAD_PARAMS_EXC("Invalid data!");
    
    prefillHeader();
    
    if(header.bitsPerSample != 16 && header.bitsPerSample != 8)
        throw UNSUPPORTED_FORMAT_EXC("Unsupported format! BPS is not 16 or 8.");
    
    int fileSizeBytes = headerSize + chanCount * (bitsPerSampleWriter/byteSize) * (int)samplesCountPerChan;
    fillParams(fileSizeBytes, sampleRate, chanCount);
}

void WavWriter::fillParams(int fileSizeBytes, int sampleRate, int chanCount)
{
    header.sampleRate = sampleRate;
    header.numChannels = chanCount;
    header.bitsPerSample = bitsPerSampleWriter;
    
    header.chunkSize = fileSizeBytes - preChunkSize;
    header.subchunk2Size = fileSizeBytes - headerSize;
    
    header.byteRate = sampleRate * chanCount * bitsPerSampleWriter/byteSize;
    header.blockAlign = chanCount * bitsPerSampleWriter/byteSize;
}

bool WavWriter::makeWAVfile(int sampleRate, DataArray &data)
{
    int chanCount = (int)data.size();
    size_t samplesCountPerChan = (int)data[0].size();
    
    try
    {
        for(const auto& it: data)
        {
            if(it.size() != samplesCountPerChan)
                throw BAD_PARAMS_EXC("Channels' sizes are not equal!");
        }
        fillHeader(chanCount, sampleRate, samplesCountPerChan);
    }
    catch(runtime_error &re)
    {
        cerr << re.what() << endl;
        return false;
    }
    
    makeData(chanCount, samplesCountPerChan, data);
    
    return true;
}

bool WavWriter::rewriteWAVfile(WavCore &WAV)
{
    if(!WAV.valid)
        return false;
    
    memcpy(&header, WAV.getHeader(), sizeof(WavCore::WavHeader));
    DataArray& data = *WAV.getData();
    int chanCount = (int)data.size();
    size_t samplesCountPerChan = (int)data[0].size();
    makeData(chanCount, samplesCountPerChan, data);
    return true;
}

void WavWriter::makeData(int chanCount, size_t samplesCountPerChan, DataArray &data)
{
    allChannels.resize(chanCount * samplesCountPerChan);
    for(int ch = 0; ch < chanCount; ch++)
    {
        const vector<short>& chData = data[ch];
        for(size_t i = 0; i < samplesCountPerChan; i++)
            allChannels[chanCount * i + ch] = chData[i];
    }
}

void WavWriter::writeWAV(const string &fileName) const
{
    try
    {
        writeOut(fileName);
        cout << "Successfully written file " << fileName << endl;
    }
    catch(runtime_error &re)
    {
        cerr << "Failed to write file: " << re.what() << endl;
    }
}

void WavWriter::writeOut(const std::string &fileName) const throw (IO_ERR_EXC, NULL_DATA)
{
    if(!maked)
        throw NULL_DATA("Nothing to write!");
    
    ofstream fout(fileName, ofstream::binary);
    
    if(fout.fail())
        throw IO_ERR_EXC("Unable to open outstream!");
    
    fout.write((char*)&header, sizeof(WavCore::WavHeader));
    fout.write((char*)allChannels.data(), header.subchunk2Size);
    
    fout.close();
}


