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
    DeckGUIGroup();
    ~DeckGUIGroup() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void loadURLintoOnePlayer(juce::File&);
    void addDeckAndPlayer(DeckGUI*);
private:
    std::set<DeckGUI*> allDeckStatus;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUIGroup)
};
