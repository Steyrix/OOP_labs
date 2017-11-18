#include "WavCore.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

const uint8_t WavCore::chunkID[4] = {'R','I','F','F'};
const uint8_t WavCore::format[4] = {'W','A','V','E'};
const uint8_t WavCore::subchunk1Id[4] = {'f','m','t',' '};
const uint8_t WavCore::subchunk2Id[4] = {'d','a','t','a'};
const uint8_t WavCore::subchunk1Size = 16;
const uint8_t WavCore::audioFormat = 1;
const uint8_t WavCore::headerSize = 44;
const uint8_t WavCore::preChunkSize = 8;
const uint8_t WavCore::byteSize = 8;

WavCore::WavCore()
{
    SetDataZero();
    SetHeaderZero();
}

WavCore::~WavCore()
{

}

void WavCore::printInfo() const
{
    cout << "----------------------------------------" << endl;
    cout << "audioFormat: "   << header.audioFormat   << endl;
    cout << "numChannels: "   << header.numChannels   << endl;
    cout << "sampleRate: "    << header.sampleRate    << endl;
    cout << "bitsPerSample: " << header.bitsPerSample << endl;
    cout << "byteRate: "      << header.byteRate      << endl;
    cout << "blockAlign: "    << header.blockAlign    << endl;
    cout << "chunkSize: "     << header.chunkSize     << endl;
    cout << "subchunk1Size: " << header.subchunk1Size << endl;
    cout << "subchunk2Size: " << header.subchunk2Size << endl;
    cout << "----------------------------------------" << endl;
}

WavCore::WavHeader* WavCore::getHeader()
{
    return &header;
}

DataArray* WavCore::getData()
{
    return &channelsData;
}

void WavCore::setHeader(const WavCore::WavHeader &hdr)
{
    header = hdr;
}

void WavCore::setData(const DataArray &dataVector)
{
    channelsData = dataVector;
}

void WavCore::SetHeaderZero()
{
    memset(&header, 0, sizeof(WavHeader));
}

void WavCore::SetDataZero()
{
    for (auto& it : channelsData)
        it.clear();
    
    channelsData.clear();
}

void WavCore::setFileSize(size_t fSize)
{
    fileSize = fSize;
}

bool WavCore::checkHeader() const
{
    try
    {
        checkAll();
        return true;
    }
    catch(runtime_error &re)
    {
        cerr << re.what() << endl;
        return false;
    }
}

void WavCore::checkAll() const
{
    checkChunkSize();
    checkRiff();
    checkWave();
    checkFMT();
    checkPCM();
    checkSubchunk1();
    checkByteRate();
    checkBlockAlign();
    checkSubchunk2();
}

void WavCore::checkRiff() const
{
    if (header.chunkId[0] != chunkID[0] ||
        header.chunkId[1] != chunkID[1] ||
        header.chunkId[2] != chunkID[2] ||
        header.chunkId[3] != chunkID[3])
        throw HEADER_RIFF_EXC("RIFF error!");
}

void WavCore::checkChunkSize() const
{
    if(header.chunkSize != fileSize - preChunkSize)
        throw HEADER_FILE_SIZE_EXC("Invalid file size!");
}

void WavCore::checkWave() const
{
    if (header.format[0] != format[0] ||
        header.format[1] != format[1] ||
        header.format[2] != format[2] ||
        header.format[3] != format[3])
        throw HEADER_WAVE_EXC("WAVE error!");
}

void WavCore::checkFMT() const
{
    if (header.subchunk1Id[0] != subchunk1Id[0] ||
        header.subchunk1Id[1] != subchunk1Id[1] ||
        header.subchunk1Id[2] != subchunk1Id[2] ||
        header.subchunk1Id[3] != subchunk1Id[3] ||
        header.subchunk2Id[0] != subchunk2Id[0] ||
        header.subchunk2Id[1] != subchunk2Id[1] ||
        header.subchunk2Id[2] != subchunk2Id[2] ||
        header.subchunk2Id[3] != subchunk2Id[3])
        throw HEADER_FMT_EXC("FMT error!");
}

void WavCore::checkPCM() const
{
    if (header.audioFormat != audioFormat)
        throw HEADER_NOT_PCM_EXC("Header is not PCM!");
}

void WavCore::checkSubchunk1() const
{
    if (header.subchunk1Size != subchunk1Size)
        throw HEADER_SUBCHUNK1_EXC("Invalid subchunk1!");
}

void WavCore::checkByteRate() const
{
    if (header.byteRate != header.sampleRate * header.numChannels * header.bitsPerSample / byteSize)
        throw HEADER_BYTERATE_EXC("Invalid byterate!");
}

void WavCore::checkBlockAlign() const
{
    if (header.blockAlign != header.numChannels * header.bitsPerSample / byteSize)
        throw HEADER_BLOCK_ALIGN_EXC("Invalid block align!");
}

void WavCore::checkSubchunk2() const 
{
    if (header.subchunk2Size != (long)fileSize - headerSize)
        throw HEADER_SUBCHUNK2_SIZE_EXC("Invalid size of subchunk2");
}











