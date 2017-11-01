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
    //mody.addReverb(WAV.getHeader()->sampleRate, 0.5f, 0.6f, WAV);
    
    getchar();
    return 0;
}
