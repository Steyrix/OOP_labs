#include "WavReader.hpp"
#include <iostream>

using namespace std;

const uint8_t WavReader::headerSize = 44;

WavReader::WavReader(const string &fileName)
{
    if(extractWavData(fileName))
        cout << "Successfully read file " << fileName << endl;
    else cout << "Failed to read " << fileName << endl;
}

WavReader::~WavReader(){}

void WavReader::ReInit(const std::string &fileName)
{
    if(extractWavData(fileName))
        cout << "Successfully read file " << fileName << endl;
    else cout << "Failed to read " << fileName << endl;
}

WavCore* WavReader::getWAV()
{
    return &WAV;
}

bool WavReader::readHeader(const string &fileName) throw (IO_ERR_EXC, BAD_FORMAT_EXC)
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
    
    fileSize = getBytesReadCount(0, fin);
    WAV.setFileSize(fileSize);
    fin.close();
    
    WAV.setHeader(headerTemp);
    return WAV.valid;
}

bool WavReader::readData(const string &fileName) throw (IO_ERR_EXC)
{
    ifstream fin(fileName, ifstream::binary);
    try
    {
        if(WAV.getHeader()->bitsPerSample != 16 && WAV.getHeader()->bitsPerSample != 8)
            throw UNSUPPORTED_FORMAT_EXC("Only 16-bit and 8-bit samples are supported!");
        
        if(fin.fail())
            throw IO_ERR_EXC("Unable to open file! " + fileName);
        
        fin.seekg(headerSize);
        
        int chanCount = WAV.getHeader()->numChannels,
        samplesPerChan = (static_cast<int>(WAV.getHeader()->subchunk2Size) / sizeof(short)) / chanCount;
        
        vector<short> allChannels;
        allChannels.resize(samplesPerChan * chanCount);
        
        fin.read((char*)allChannels.data(), WAV.getHeader()->subchunk2Size);
        
        size_t bytesRead = fin.gcount();
        size_t bytesReadCount = getBytesReadCount(headerSize, fin);
        if(bytesRead != bytesReadCount)
            throw IO_ERR_EXC("Function extractWavData read only " + to_string(bytesRead) + " of " + to_string(bytesReadCount));
        
        fin.close();
        
        separateChannelsData(allChannels, chanCount, samplesPerChan);
        WAV.setData(dataToSet);
        
        return true;
    }
    catch(runtime_error &re)
    {
        cerr << re.what() << endl;
        return false;
    }
}

void WavReader::separateChannelsData(const vector<short>& allChannels, int chanCount, int samplesPerChan)
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


bool WavReader::extractWavData(const string &fileName) throw (IO_ERR_EXC, UNSUPPORTED_FORMAT_EXC)
{
    bool result;
    
    try
    {
        result = readHeader(fileName) && readData(fileName);
    }
    catch (runtime_error &re)
    {
        cerr << re.what() << endl;
        return false;
    }
    
    return result;
}

