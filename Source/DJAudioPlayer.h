/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 27 Jan 2022 10:31:38pm
    Author:  matt

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer: public juce::AudioSource{
    public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double positionInSec);
    void setRelativePosition(double position);

    void start();
    void stop();

    //get relative position of playhead
    double getPositionRelative();

    private:
        juce::AudioFormatManager& formatManager;

        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

        juce::AudioTransportSource transportSource;

        juce::ResamplingAudioSource resampleSource{&transportSource, false,2};
};
