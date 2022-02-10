/*
  ==============================================================================

    WaveformDisplay.h
    Created: 8 Feb 2022 4:04:25pm
    Author:  matt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay : public juce::Component,
                        public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    void loadURL(juce::URL audioURL);

    //get the relative position of playhead
    void setPositionRelative(double position);

private:
    juce::AudioThumbnail audioThumbnail;
    bool isFileLoaded;
    double localPosition;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
