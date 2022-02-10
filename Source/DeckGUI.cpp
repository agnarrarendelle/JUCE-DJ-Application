/*
  ==============================================================================

    DeckGUI.cpp
    Created: 8 Feb 2022 12:47:03am
    Author:  matt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse
                 ): player(_player),
                    waveformDisplay(formatManagerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);

    volumeSlider.setRange(0,1);
    positionSlider.setRange(0,1);
    speedSlider.setRange(0,10);

    startTimer(100);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
//
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
//
//    g.setColour (juce::Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
//
//    g.setColour (juce::Colours::white);
//    g.setFont (14.0f);
//    g.drawText ("DeckGUI", getLocalBounds(),
//                juce::Justification::centred, true);   // draw some placeholder text

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void DeckGUI::resized()
{
    double rowHeight = getHeight()/10;
    playButton.setBounds(0,0,getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    waveformDisplay.setBounds(0, rowHeight*2, getWidth(), rowHeight*2);
    loadButton.setBounds(0, rowHeight*4, getWidth(), rowHeight);
    volumeSlider.setBounds(0, rowHeight*6, getWidth(), rowHeight);
    speedSlider.setBounds(0, rowHeight*7, getWidth(), rowHeight);
    positionSlider.setBounds(0, rowHeight*8, getWidth(), rowHeight);


}

void DeckGUI::buttonClicked(juce::Button * button) {
    if(button == &playButton){
        //isPlaying = true;
        player->start();
    }else if(button == &stopButton){
        //isPlaying = false;
        player->stop();
    }else if(button == &loadButton){
        juce::FileChooser chooser{"Select a audio file to play..."};
        if(chooser.browseForFileToOpen()){
            player->loadURL(juce::URL{chooser.getResult()});
            waveformDisplay.loadURL(juce::URL{chooser.getResult()});
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider *slider) {
    double value = slider->getValue();
    if(slider == &volumeSlider){
        //gain = value;
        player->setGain(value);
    }else if(slider == &speedSlider){
        player->setSpeed(value);
        //resampleSource.setResamplingRatio(slider->getValue());
    }else if(slider == &positionSlider){
        player->setRelativePosition(value);
        //transportSource.setPosition(positionSlider.getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files){
    std::cout<<"File Dragging"<<std::endl;
    return 1;
}
void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y){
    if(files.size() == 1){
        std::cout<<"File "<< files[0] <<" Dropped"<<std::endl;
        juce::URL fileURL{juce::File{files[0]}};
        player->loadURL(fileURL);
    }
}

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
