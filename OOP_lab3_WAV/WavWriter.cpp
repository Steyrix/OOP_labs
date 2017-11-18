#include "WavWriter.hpp"
#include <iostream>

using namespace std;

void WavWriter::createSoundFile(const string &fileName, WavCore &soundFile)
{
    if(soundFile.getData()->empty())
        throw NULL_DATA("Nothing to write!");
    
    if(!soundFile.checkHeader())
        throw BAD_PARAMS_EXC("Invalid header!");
    
    ofstream fout(fileName, ofstream::binary);
    
    if(fout.fail())
        throw IO_ERR_EXC("Unable to open outstream!");
    
    WavCore::WavHeader header = *soundFile.getHeader();
    DataArray data = *soundFile.getData();
    vector<short> allChannels = makeData(header.numChannels, data[0].size(), data);
    
    fout.write((char*)&header, sizeof(WavCore::WavHeader));
    fout.write((char*)allChannels.data(), header.subchunk2Size);
    
    fout.close();
}

WavWriter::~WavWriter(){}

vector<short> WavWriter::makeData(int chanCount, size_t samplesCountPerChan, DataArray &data)
{
    vector<short> allChannels;
    allChannels.resize(chanCount * samplesCountPerChan);
    for(int ch = 0; ch < chanCount; ch++)
    {
        const vector<short>& chData = data[ch];
        for(size_t i = 0; i < samplesCountPerChan; i++)
            allChannels[chanCount * i + ch] = chData[i];
    }
    return allChannels;
}



