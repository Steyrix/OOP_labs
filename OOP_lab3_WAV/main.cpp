#include "WavWriter.hpp"
#include "WavReader.hpp"
#include "WavModifyer.hpp"

int main()
{
    WavReader reader("0.wav");
    WavCore WAV = *reader.getWAV();
    
    WavWriter writer;
    writer.ReInit(WAV);
    writer.writeWAV("0new.wav");
    
    WavModifyer mody;
    mody.cutFromEnding(30.0f, WAV);
    mody.cutFromBeginning(10.0f, WAV);
    WavCore WAV2 = WAV;
    mody.setModifyingState(WavModifyer::ModifyState::SAVE);
    mody.makeMono(WAV2);
    mody.setModifyingState(WavModifyer::ModifyState::SAVE_AS_NEW);
    mody.addReverb(WAV2.getHeader()->sampleRate, 0.5f, 0.6f, WAV2);
    getchar();
    return 0;
}
