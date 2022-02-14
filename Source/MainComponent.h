#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include <map>
#include <vector>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

//    //implement Button::Listener pure virtual function
//    void buttonClicked(juce::Button*) override;
//    //implement Slider::Listener pure virtual function
//    void sliderValueChanged(juce::Slider* slider) override;
private:
    //==============================================================================
    // Your private member variables go here...
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{100};

    DJAudioPlayer player1{formatManager};
    DJAudioPlayer player2{formatManager};
    DJAudioPlayer player3{formatManager};


    juce::MixerAudioSource mixerSource;

    double ff;
    double gain;
    bool isPlaying;
    DeckGUIGroup deckGUIGroup{formatManager, thumbnailCache, player1, player2, player3};
    PlaylistComponent playlistComponent{formatManager, deckGUIGroup};
    std::vector<DJAudioPlayer*> allPlayers;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
