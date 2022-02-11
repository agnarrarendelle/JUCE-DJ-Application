/*
  ==============================================================================

    PlaylistComponent.h
    Created: 9 Feb 2022 11:03:20pm
    Author:  matt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <set>
#include <map>
#include "DeckGUI.h"
//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::FileDragAndDropTarget
{
public:
    PlaylistComponent(juce::AudioFormatManager&, DeckGUI&);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground (
            juce::Graphics& g,
            int rowNumber,
            int width,
            int height,
            bool rowIsSelected
    ) override;

    void paintCell (
            juce::Graphics& g,
            int rowNumber,
            int columnId,
            int width,
            int height,
            bool rowIsSelected
    ) override;

    juce::Component* refreshComponentForCell (
            int rowNumber,
            int columnId,
            bool isRowSelected,
            juce::Component* existingComponentToUpdate
    ) override;

    void buttonClicked(juce::Button*) override;

    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    void filesDropped (const juce::StringArray &files, int x, int y) override;


private:
    juce::AudioFormatManager& formatManager;
    juce::TableListBox tableComponent;
    std::vector<std::string> trackTitles;
    std::set<juce::File> allTracks;
    std::map<std::string, juce::File> fileStatus;

    DeckGUI& deckGui;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
