#pragma once
#include "WavCore.hpp"
#include "WavWriter.hpp"

class WavModifyer
{
public:
    enum ModifyState
    {
        SAVE,
        SAVE_AS_NEW
    };
    WavModifyer();
    ~WavModifyer();
    void cutFromEnding(float seconds, WavCore &WAV);
    void cutFromBeginning(float seconds, WavCore &WAV);
    void addReverb(int sampleRate, double delaySeconds, float decay, WavCore &WAV);
    void setModifyingState(ModifyState state);
    void saveFileAsNew(WavCore &WAV, const std::string &fileName);
private:
    int cnt;
    size_t getChanCount(const DataArray &soundData) const;
    size_t getSamplesPerChan(const DataArray &soundData) const;
    void processSignal(size_t samplesCountPerChan, size_t chanCount, int delaySamples, float decay, DataArray &data);
    void convertSignal(size_t i, size_t samplesCountPerChan, std::vector<float> signal, DataArray &data);
    void attachReverb(size_t samplesCountPerChan, int delaySamples, float decay, std::vector<float> signal);
    void scaleToShort(size_t i, size_t samplesCountPerChan, float maxMagnitude, std::vector<float> signal, DataArray &data);
    void updateHeader(WavCore &WAV, size_t chanCount, int change);
    float findMaxMagnitude(size_t samplesCountPerChan, int delaySamples, std::vector<float> signal);
    int convertSecondsToBytes(float secs, int bitsPerSample, int sampleRate) const;
    WavWriter writer;
    ModifyState currState;
};
