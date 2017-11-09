#include "WavModifyer.hpp"
#include <iostream>
#include <cmath>
using namespace std;

WavModifyer::WavModifyer()
{
    currState = WavModifyer::ModifyState::SAVE_AS_NEW;
    cnt = 0;
}

void WavModifyer::setModifyingState(WavModifyer::ModifyState state)
{
    currState = state;
}

WavModifyer::~WavModifyer(){}

void WavModifyer::addReverb(int sampleRate, double delaySeconds, float decay, WavCore &WAV)
{
    WavCore copy = WAV;
    DataArray& soundData = *copy.getData();
    try
    {
        size_t chanCount = getChanCount(soundData);
        size_t samplesCountPerChan = getSamplesPerChan(soundData);
        int delaySamples = (int)(delaySeconds * sampleRate);
        processSignal(samplesCountPerChan, chanCount, delaySamples, decay, soundData);
        if(currState == WavModifyer::ModifyState::SAVE_AS_NEW)
            saveFileAsNew(copy, "wav" + to_string(cnt++) + "reverbed.wav");
        else WAV = copy;
    }
    catch(runtime_error &re)
    {
        cerr << "Failed to add reverb: \n" << re.what() << endl;
    }

}

size_t WavModifyer::getChanCount(const DataArray &soundData) const throw (BAD_PARAMS_EXC)
{
    size_t chanCount = soundData.size();
    if(chanCount < 1)
        throw BAD_PARAMS_EXC("Invalid count of channels!");
    
    return chanCount;
}

size_t WavModifyer::getSamplesPerChan(const DataArray &soundData) const throw (BAD_PARAMS_EXC)
{
    size_t samplesCountPerChan = (size_t)soundData[0].size();
    for (const auto &it : soundData)
        if (it.size() != samplesCountPerChan)
            throw BAD_PARAMS_EXC("Channels don't have the same number of samples!");
    
    return samplesCountPerChan;
}

void WavModifyer::processSignal(size_t samplesCountPerChan, size_t chanCount, int delaySamples, float decay, DataArray &data)
{

    for(size_t ch = 0; ch < chanCount; ch++)
    {
        vector<float> tmp;
        tmp.resize(data[ch].size());
        convertSignal(ch, samplesCountPerChan, tmp, data);
        attachReverb(samplesCountPerChan, delaySamples, decay, tmp);
        float maxMagnitude = findMaxMagnitude(samplesCountPerChan, delaySamples, tmp);
        scaleToShort(ch, samplesCountPerChan, maxMagnitude, tmp, data);
    }
}

void WavModifyer::convertSignal(size_t i, size_t samplesCountPerChan, vector<float> &signal, DataArray &data)
{
    for(size_t j = 0; j < samplesCountPerChan; j++)
        signal[j] = data[i][j];
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

void WavModifyer::scaleToShort(size_t i, size_t samplesCountPerChan, float maxMagnitude, vector<float> &signal, DataArray &data)
{
    float normCoef = 30000.0f / maxMagnitude;
    for(size_t j = 0; j < samplesCountPerChan; j++)
        data[i][j] = (short)(normCoef * signal[j]);

}

void WavModifyer::saveFileAsNew(WavCore &WAV, const string &fileName)
{
    writer.ReInit(WAV);
    writer.writeWAV(fileName);
}

int WavModifyer::convertSecondsToBytes(float secs, int bitsPerSample, int sampleRate) const
{
    return ((bitsPerSample / 8) * sampleRate * secs) / sizeof(short);
}


void WavModifyer::cutFromEnding(float seconds, WavCore &WAV)
{
    WavCore copy = WAV;
    DataArray& soundData = *copy.getData();
    int bytesToDelete = convertSecondsToBytes(seconds, copy.getHeader()->bitsPerSample, copy.getHeader()->sampleRate);
    for(auto &it: soundData)
    {
        it.erase(it.end()-bytesToDelete, it.end());
    }
    updateHeader(copy);
    
    saveFileAsNew(copy, "cutted_from_end.wav");
}

void WavModifyer::cutFromBeginning(float seconds, WavCore &WAV)
{
    WavCore copy = WAV;
    DataArray& soundData = *copy.getData();
    int bytesToDelete = convertSecondsToBytes(seconds, copy.getHeader()->bitsPerSample, copy.getHeader()->sampleRate);
    for(auto &it: soundData)
    {
        it.erase(it.begin(), it.begin() + bytesToDelete);
    }
    updateHeader(copy);
    
    saveFileAsNew(copy, "cutted_from_begin.wav");
}

void WavModifyer::makeMono(WavCore &WAV)
{
    WavCore copy = WAV;
    DataArray &source = *copy.getData();
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
    
    if(currState == WavModifyer::ModifyState::SAVE_AS_NEW)
    {
        copy.setData(monoData);
        updateHeader(copy);
        saveFileAsNew(copy, "MonoWAV.wav");
    }
    else
    {
        WAV.setData(monoData);
        updateHeader(WAV);
    }
}

void WavModifyer::calculateMean(DataArray &source, DataArray &dest, size_t samplesCountPerChan)
{
    dest[0].resize(samplesCountPerChan);
    for(size_t i = 0; i < samplesCountPerChan; i++)
        dest[0][i] = (source[0][i] + source[1][i]) / 2;
}

void WavModifyer::checkChannels(DataArray &data) const throw (BAD_PARAMS_EXC)
{
    size_t chanCount = data.size();
    if(chanCount != 2)
        throw BAD_PARAMS_EXC("Wrong number of channels! Unable to make mono");
}


void WavModifyer::updateHeader(WavCore &WAV)
{
    size_t sizeSubchunk = 0;
    for(auto &it: *WAV.getData())
        sizeSubchunk += it.size();
    WAV.getHeader()->numChannels = WAV.getData()->size();
    WAV.getHeader()->subchunk2Size = ((unsigned int)sizeSubchunk * sizeof(short));
    WAV.getHeader()->chunkSize = 36 + WAV.getHeader()->subchunk2Size;
}




