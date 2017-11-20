#include "WavWriter.hpp"
#include "WavReader.hpp"
#include "WavModifyer.hpp"
#include <iostream>

int main()
{
    try
    {
        WavCore soundData = WavReader::readSoundFile("0.wav");
        
        soundData.printInfo();
        WavCore soundData2 = WavReader::readSoundFile("0.wav");
        WavWriter::createSoundFile("0rewrited00.wav", soundData);
        WavModifyer::makeMono(soundData);
        
        soundData.printInfo();
        
        WavModifyer::addReverb(soundData, 0.5f, 0.6);
        WavModifyer::addReverb(soundData2, 0.5f, 0.6);
        WavWriter::createSoundFile("0reverbed.wav", soundData);
        WavWriter::createSoundFile("reverbedStereo.wav", soundData2);
        
    }
    catch(std::runtime_error &re)
    {
        std::cerr << re.what() << std::endl;
    }
    
    std::cout << "Processes finished..." << std::endl;
    getchar();
    
    return 0;
}
