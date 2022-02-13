/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 27 Jan 2022 10:31:38pm
    Author:  matt

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
:formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer(){

}
//==============================================================================
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate){
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected,sampleRate);

    transportSource.setGain(0.5);
}
void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill){
    resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources(){
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL){
    //reading the numbers from the audio file
    juce::AudioFormatReader* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if(reader != nullptr){
        //use unique_ptr so that when the function exists, the content on the heap will be freed
        //pass audioFormatReader* reader to read the file
        //pass true so that it should destruct reader when it has finished reading it
        std::unique_ptr<juce::AudioFormatReaderSource> newSource{(new juce::AudioFormatReaderSource(reader, true))};
        transportSource.setSource(
                newSource.get(),
                0,
                nullptr,
                reader->sampleRate
        );
        //once the input audio source has been successfully set,
        //release the source from newSource to readerSource
        readerSource.reset(newSource.release());
    }else{
        std::cout<<"Loading File Failed"<<std::endl;
    }
}
void DJAudioPlayer::setGain(double gain){
    if(gain < 0 || gain > 1){
        std::cout<<"gain must be between 0 and 1"<<std::endl;
    }else{
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio){
    if(ratio < 0 || ratio > 100){
        std::cout<<"ratio must be between 0 and 100"<<std::endl;
    }else{
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setRelativePosition(double position) {
    if(position < 0 || position > 1){
        std::cout<<"position must be between 0 and 1"<<std::endl;
    }else{
        double positionInSec = transportSource.getLengthInSeconds() * position;
        setPosition(positionInSec);
    }
}

void DJAudioPlayer::setPosition(double positionInSec){
    transportSource.setPosition(positionInSec);
}

void DJAudioPlayer::start(){
    transportSource.start();
}
void DJAudioPlayer::stop(){
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative(){
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getPosition() {
    return transportSource.getCurrentPosition();
}

bool DJAudioPlayer::isTrackFinished(){
    return ((int)getPositionRelative()) == 1;
}
