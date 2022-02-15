/*
  ==============================================================================

    DeckGUI.h
    Created: 8 Feb 2022 12:47:03am
    Author:  matt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include <vector>

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* _player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //implement Button::Listener pure virtual function
    void buttonClicked(juce::Button*) override;
    //implement Slider::Listener pure virtual function
    void sliderValueChanged(juce::Slider* slider) override;

    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;

    void timerCallback() override;

    void loadURL(juce::File &);

    bool isTrackLoaded();

    bool isTrackEnded();

    void trackStatesInitialized();
private:
    juce::TextButton playButton{"Play"};
    juce::TextButton stopButton{"Stop"};
    juce::TextButton loadButton{"Load"};
    juce::TextButton fastForwardButton{">> 3 seconds"};
    juce::TextButton rewindButton{"3 seconds <<"};
    juce::Slider volumeSlider{"gain"};
    juce::Slider speedSlider{"speed"};
    juce::Slider positionSlider{"position"};
    juce::LookAndFeel_V4 lookAndFeel;
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    bool isLoaded;
    std::vector<juce::Slider*> allSliders;
    std::vector<juce::TextButton*> allButtons;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
