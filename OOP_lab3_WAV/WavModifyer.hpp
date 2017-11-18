#pragma once
#include "WavCore.hpp"

class WavModifyer
{
public:
    WavModifyer();
    ~WavModifyer();
    static void cutFromEnding(float seconds, WavCore &soundFile);
    static void cutFromBeginning(float seconds, WavCore &soundFile);
    static void addReverb(WavCore &soundFile, double delaySeconds, float decay);
    static void makeMono(WavCore &soundFile);
private:
    static size_t getChanCount(const DataArray &soundData);
    static size_t getSamplesPerChan(const DataArray &soundData);
    static void processSignal(size_t samplesCountPerChan, int delaySamples, float decay, DataArray &data,
                              short channelNumber);
    static void attachReverb(size_t samplesCountPerChan, int delaySamples, float decay, std::vector<float> &signal);
    static void checkChannels(DataArray& data);
    static void calculateMean(DataArray& source, DataArray &dest, size_t samplesCountPerChan);
    static void updateHeader(WavCore &soundFile);
    static float findMaxMagnitude(size_t samplesCountPerChan, int delaySamples, std::vector<float> &signal);
    static int convertSecondsToBytes(float secs, int bitsPerSample, int sampleRate);
};
