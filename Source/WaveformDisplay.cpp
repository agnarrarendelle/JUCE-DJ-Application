/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 8 Feb 2022 4:04:25pm
    Author:  matt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse
                                 ): audioThumbnail(1000,formatManagerToUse, cacheToUse),
                                    localPosition(0),
                                    isFileLoaded(false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::black);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 3);   // draw an outline around the component
    g.setColour (juce::Colours::mediumseagreen);

    if(isFileLoaded){
        audioThumbnail.drawChannel(
                g,
                getLocalBounds(),
                0,
                audioThumbnail.getTotalLength(),
                0,
                1.0f
        );
        g.setColour(juce::Colours::lightblue);
        g.drawRect(localPosition * getWidth(), 0, 1, getHeight());
        g.drawRect(localPosition * getWidth(), 0, 4, getHeight());
        g.drawRect(localPosition * getWidth(), 0, 7, getHeight());


    }else{
        g.setFont (juce::Font{20.0f, juce::Font::bold});
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(juce::URL audioURL) {
    audioThumbnail.clear();
    isFileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
    if(isFileLoaded){
        std::cout<<"Waveform URL loaded"<<std::endl;
    }else{
        std::cout<<"Waveform URL failed to be loaded"<<std::endl;
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source) {
    repaint();
}

void WaveformDisplay::setPositionRelative(double position) {
    if(position != localPosition){
        localPosition = position;
        repaint();
    }
}
