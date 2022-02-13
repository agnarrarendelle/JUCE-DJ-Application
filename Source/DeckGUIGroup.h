/*
  ==============================================================================

    DeckGUIGroup.h
    Created: 12 Feb 2022 4:31:00pm
    Author:  matt

  ==============================================================================
*/

#pragma once

#include <vector>
#include <JuceHeader.h>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include <set>
//==============================================================================
/*
*/
class DeckGUIGroup  : public juce::Component
{
public:
    DeckGUIGroup(juce::AudioFormatManager& _formatManager,
                 juce::AudioThumbnailCache& _cache,
                 DJAudioPlayer& _player1,
                 DJAudioPlayer& _player2);
    ~DeckGUIGroup() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void loadURLintoOnePlayer(juce::File&);
    void addDeckAndPlayer(DeckGUI*);
private:
    juce::AudioFormatManager& formatManager;
    juce::AudioThumbnailCache& thumbnailCache;
    DJAudioPlayer& player1;
    DJAudioPlayer& player2;

    DeckGUI deckGUI1{&player1, formatManager, thumbnailCache};
    DeckGUI deckGUI2{&player2, formatManager, thumbnailCache};
    std::set<DeckGUI*> allDeckStatus;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUIGroup)
};
