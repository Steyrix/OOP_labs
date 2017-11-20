#include "WavModifyer.hpp"
#include <iostream>
#include <cmath>
using namespace std;

WavModifyer::~WavModifyer(){}

size_t WavModifyer::getChanCount(const DataArray &soundData)
{
    size_t chanCount = soundData.size();
    if(chanCount != 1 && chanCount != 2)
        throw BAD_PARAMS_EXC("Invalid count of channels!");
    
    return chanCount;
}

size_t WavModifyer::getSamplesPerChan(const DataArray &soundData)
{
    size_t samplesCountPerChan = (size_t)soundData[0].size();
    for (const auto &it : soundData)
        if (it.size() != samplesCountPerChan)
            throw BAD_PARAMS_EXC("Channels don't have the same number of samples!");
    
    return samplesCountPerChan;
}

void WavModifyer::addReverb(WavCore &soundFile, double delaySeconds, float decay)
{
    DataArray& soundData = *soundFile.getData();
    int sampleRate = soundFile.getHeader()->sampleRate;
    size_t chanCount = getChanCount(soundData);
    size_t samplesCountPerChan = getSamplesPerChan(soundData);
    int delaySamples = (int)(delaySeconds * sampleRate);
    if(chanCount == 1)
        processSignal(samplesCountPerChan, delaySamples, decay, soundData, 0);
    else
    {
        processSignal(samplesCountPerChan, delaySamples, decay, soundData, 0);
        processSignal(samplesCountPerChan,  (int)(delaySamples * 0.75f), (decay * 1.25f), soundData, 1);
    }
}

void WavModifyer::processSignal(size_t samplesCountPerChan, int delaySamples, float decay, DataArray &data,
                                short channelNumber)
{
        int cnt = 0;
        vector<float> tmp;
        tmp.resize(data[channelNumber].size());
        //convertSignal Lambda
        for_each(tmp.begin(), tmp.end(), [&](float &val){val = data[0][cnt++];});
        attachReverb(samplesCountPerChan, delaySamples, decay, tmp);
        cnt = 0;
        float maxMagnitude = findMaxMagnitude(samplesCountPerChan, delaySamples, tmp),
        normCoef = 30000.0f/maxMagnitude;
        //ScaleToShort Lambda
        for_each(data[channelNumber].begin(), data[channelNumber].end(), [&](short &val)
                 {val = (short)(normCoef * tmp[cnt++]);});
}

void WavModifyer::attachReverb(size_t samplesCountPerChan, int delaySamples, float decay, vector<float> &signal)
{
    for(size_t i = 0; i < samplesCountPerChan - delaySamples; i++)
        signal[i+delaySamples] += (decay * signal[i]);
}

float WavModifyer::findMaxMagnitude(size_t samplesCountPerChan, int delaySamples, vector<float> &signal)
{
    float maxMagnitude = 0.0f;
    for(size_t i = 0; i < samplesCountPerChan - delaySamples; i++)
        if(abs(signal[i]) > maxMagnitude)
            maxMagnitude = abs(signal[i]);
    
    return maxMagnitude;
}

int WavModifyer::convertSecondsToBytes(float secs, int bitsPerSample, int sampleRate)
{
    return ((bitsPerSample / 8) * sampleRate * secs) / sizeof(short);
}


void WavModifyer::cutFromEnding(float seconds, WavCore &soundFile)
{
    DataArray& soundData = *soundFile.getData();
    int bytesToDelete = convertSecondsToBytes(seconds, soundFile.getHeader()->bitsPerSample, soundFile.getHeader()->sampleRate);
    for(auto &it: soundData)
        it.erase(it.end()-bytesToDelete, it.end());
    
    updateHeader(soundFile);
}

void WavModifyer::cutFromBeginning(float seconds, WavCore &soundFile)
{
    DataArray& soundData = *soundFile.getData();
    int bytesToDelete = convertSecondsToBytes(seconds, soundFile.getHeader()->bitsPerSample, soundFile.getHeader()->sampleRate);
    for(auto &it: soundData)
        it.erase(it.begin(), it.begin() + bytesToDelete);
    
    updateHeader(soundFile);
}

void WavModifyer::makeMono(WavCore &soundFile)
{
    DataArray &source = *soundFile.getData();
    size_t samplesCountPerChan;
    try
    {
        checkChannels(source);
        samplesCountPerChan = getSamplesPerChan(source);
    }
    catch(runtime_error &re)
    {
        cerr << "Failed to make mono: " << re.what() << endl;
    }
    DataArray monoData(1);
    calculateMean(source, monoData, samplesCountPerChan);

    soundFile.setData(monoData);
    updateHeader(soundFile);
}

void WavModifyer::calculateMean(DataArray &source, DataArray &dest, size_t samplesCountPerChan)
{
    dest[0].resize(samplesCountPerChan);
    for(size_t i = 0; i < samplesCountPerChan; i++)
        dest[0][i] = (source[0][i] + source[1][i]) / 2;
}

void WavModifyer::checkChannels(DataArray &data)
{
    size_t chanCount = data.size();
    if(chanCount != 2)
        throw BAD_PARAMS_EXC("Wrong number of channels! Unable to make mono");
}


void WavModifyer::updateHeader(WavCore &soundFile)
{
    size_t sizeSubchunk = 0;
    for(auto &it: *soundFile.getData())
        sizeSubchunk += it.size();
    soundFile.getHeader()->numChannels = soundFile.getData()->size();
    soundFile.getHeader()->subchunk2Size = ((unsigned int)sizeSubchunk * sizeof(short));
    soundFile.getHeader()->chunkSize = WavCore::headerSize - WavCore::preChunkSize + soundFile.getHeader()->subchunk2Size;
    soundFile.setFileSize(WavCore::headerSize+soundFile.getHeader()->subchunk2Size);
    soundFile.getHeader()->byteRate = soundFile.getHeader()->sampleRate * soundFile.getHeader()->numChannels * soundFile.getHeader()->bitsPerSample / WavCore::byteSize;
    soundFile.getHeader()->blockAlign = soundFile.getHeader()->numChannels * soundFile.getHeader()->bitsPerSample / WavCore::byteSize;
}

