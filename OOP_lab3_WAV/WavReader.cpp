#include "WavReader.hpp"
#include <iostream>

using namespace std;

const uint8_t WavReader::headerSize = 44;

WavCore WavReader::readSoundFile(const string& fileName)
{
    WavCore soundFile;
    readHeader(fileName, soundFile);
    readData(fileName, soundFile);
    return soundFile;
}

WavReader::~WavReader(){}

void WavReader::readHeader(const string &fileName, WavCore &destinationFile)
{
    
    ifstream fin(fileName, ifstream::binary);
    if(fin.fail())
        throw IO_ERR_EXC("Unable to open file! " + fileName);
    
    WavCore::WavHeader headerTemp;
    setTempZero(headerTemp);
    
    fin.seekg(0, ios::beg);
    fin.read((char*)&headerTemp, sizeof(WavCore::WavHeader));
    
    if(fin.gcount() != sizeof(WavCore::WavHeader))
        throw BAD_FORMAT_EXC("File has bad format and fails to be read correctly!");
    
    size_t fileSize = getBytesReadCount(0, fin);
    destinationFile.setFileSize(fileSize);
    fin.close();
    
    destinationFile.setHeader(headerTemp);
    destinationFile.checkHeader();
}

void WavReader::readData(const string &fileName, WavCore &destinationFile)
{
    ifstream fin(fileName, ifstream::binary);

    if(destinationFile.getHeader()->bitsPerSample != 16 && destinationFile.getHeader()->bitsPerSample != 8)
        throw UNSUPPORTED_FORMAT_EXC("Only 16-bit and 8-bit samples are supported!");
        
    if(fin.fail())
        throw IO_ERR_EXC("Unable to open file! " + fileName);
        
    fin.seekg(headerSize);
        
    int chanCount = destinationFile.getHeader()->numChannels,
    samplesPerChan = (static_cast<int>(destinationFile.getHeader()->subchunk2Size) / sizeof(short)) / chanCount;
        
    vector<short> allChannels;
    allChannels.resize(samplesPerChan * chanCount);
        
    fin.read((char*)allChannels.data(), destinationFile.getHeader()->subchunk2Size);
        
    size_t bytesRead = fin.gcount();
    size_t bytesReadCount = getBytesReadCount(headerSize, fin);
    if(bytesRead != bytesReadCount)
        throw IO_ERR_EXC("Function extractWavData read only " + to_string(bytesRead) + " of " + to_string(bytesReadCount));
        
    fin.close();
        
    DataArray dataToSet;
    separateChannelsData(allChannels, dataToSet, chanCount, samplesPerChan);
    destinationFile.setData(dataToSet);
    
}

void WavReader::separateChannelsData(const vector<short>& allChannels, DataArray &dataToSet, int chanCount, int samplesPerChan)
{
    dataToSet.resize(chanCount);
    for(auto &it : dataToSet)
        it.resize(samplesPerChan);
    
    for(int ch = 0; ch < chanCount; ch++)
    {
        vector<short>& chData = dataToSet[ch];
        for(size_t i = 0; i < samplesPerChan; i++)
            chData[i] = allChannels[chanCount * i + ch];
    }
}

void WavReader::setTempZero(WavCore::WavHeader &headerTemp)
{
    memset(&headerTemp, 0, sizeof(WavCore::WavHeader));
}

streampos WavReader::getBytesReadCount(streampos pos, ifstream &stream)
{
    streampos fSize;
    stream.seekg(0, ios::end);
    fSize = stream.tellg() - pos;
    
    return fSize;
}


