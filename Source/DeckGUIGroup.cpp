/*
  ==============================================================================

    DeckGUIGroup.cpp
    Created: 12 Feb 2022 4:31:00pm
    Author:  matt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUIGroup.h"

//==============================================================================
DeckGUIGroup::DeckGUIGroup(juce::AudioFormatManager& _formatManager,
                           juce::AudioThumbnailCache& _cache,
                           DJAudioPlayer& _player1,
                           DJAudioPlayer& _player2)
:formatManager(_formatManager),
thumbnailCache(_cache),
player1(_player1),
player2(_player2)

{
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addDeckAndPlayer(&deckGUI1);
    addDeckAndPlayer(&deckGUI2);
}

DeckGUIGroup::~DeckGUIGroup()
{
}

void DeckGUIGroup::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUIGroup", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUIGroup::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    deckGUI1.setBounds(0,0,getWidth()/2, getHeight());
    deckGUI2.setBounds(getWidth()/2,0, getWidth()/2, getHeight());

}

void DeckGUIGroup::addDeckAndPlayer(DeckGUI *deckGUI) {
    allDeckStatus.insert(deckGUI);
}

void DeckGUIGroup::loadURLintoOnePlayer(juce::File & file) {
    for(auto const& eachDeck: allDeckStatus){
        if( eachDeck->isTrackLoaded() == false || eachDeck->isTrackEnded()){
            //eachDeck->isPlaying = true;
            eachDeck->loadURL(file);
            break;
        }
    }
}
