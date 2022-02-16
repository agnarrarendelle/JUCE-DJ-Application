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
                    waveformDisplay(formatManagerToUse, cacheToUse),
                    isLoaded(false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    allSliders.push_back(&volumeSlider);
    allSliders.push_back(&speedSlider);
    allSliders.push_back(&positionSlider);
    allButtons.push_back(&playButton);
    allButtons.push_back(&stopButton);
    allButtons.push_back(&fastForwardButton);
    allButtons.push_back(&rewindButton);
    slidersInitialization();
    buttonsInitialization();

    addAndMakeVisible(waveformDisplay);
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
    waveformDisplay.setBounds(0, 0, getWidth(), getHeight()*0.3);
    playButton.setBounds(getWidth()*0.2,getHeight()*0.32,getWidth()*0.2, getHeight()*0.15);
    stopButton.setBounds((getWidth()*0.6), getHeight()*0.32, getWidth()*0.2, getHeight()*0.15);
    volumeSlider.setBounds(getWidth()*0.2, getHeight()*0.45, getWidth()/8, getHeight()*0.5);
    speedSlider.setBounds(getWidth()*0.44, getHeight()*0.45, getWidth()/8, getHeight()*0.5);
    positionSlider.setBounds(getWidth()*0.68, getHeight()*0.45, getWidth()/8, getHeight()*0.5);
    rewindButton.setBounds(getWidth()*0.05, getHeight()*0.50, getWidth()*0.1, getHeight()*0.2);
    fastForwardButton.setBounds(getWidth()*0.85, getHeight()*0.50, getWidth()*0.1, getHeight()*0.2);
    //loadButton.setBounds(getWidth()*0.68, getHeight()*0.50, getWidth()*0.1, getHeight()*0.2);



}

void DeckGUI::buttonClicked(juce::Button * button) {
    if(button == &playButton){

        player->start();
    }else if(button == &stopButton){
        isLoaded = false;
        player->stop();
    }else if(button == &fastForwardButton){
        fastForwardButtonClicked();
    }else if(button == &rewindButton){
        rewindButtonClicked();
    }
}

void DeckGUI::sliderValueChanged(juce::Slider *slider) {
    double value = slider->getValue();
    if(slider == &volumeSlider){
        player->setGain(value);
    }else if(slider == &speedSlider){
        player->setSpeed(value);
    }else if(slider == &positionSlider){
        player->setRelativePosition(value);
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files){
    return 1;
}
void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y){
    if(files.size() == 1){
        juce::URL fileURL{juce::File{files[0]}};
        player->loadURL(fileURL);
    }
}

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::loadURL(juce::File &file) {
    std::cout<<"loaded!"<<std::endl;

    juce::URL fileURL{juce::File{file}};
    trackStatesInitialized();
    player->loadURL(fileURL);
    waveformDisplay.loadURL(fileURL);
    isLoaded = true;
}



bool DeckGUI::isTrackLoaded() {
    return isLoaded;
}

bool DeckGUI::isTrackEnded() {
    return player->isTrackFinished();
}

void DeckGUI::trackStatesInitialized(){
    player->setRelativePosition(0);
    player->setGain(0.5);
    positionSlider.setValue(0);
    volumeSlider.setValue(0.5);
}

void DeckGUI::slidersInitialization() {
    for(const auto& slider: allSliders){
        slider->setLookAndFeel(&lookAndFeel);
        slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
        slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 40);
        addAndMakeVisible(*slider);
        slider->addListener(this);
    }
    volumeSlider.textFromValueFunction = [](double value)
    {
        return juce::String("volume");
    };
    speedSlider.textFromValueFunction = [](double value)
    {
        return juce::String("speed");
    };
    positionSlider.textFromValueFunction = [](double value)
    {
        return juce::String("position");
    };
    volumeSlider.setRange(0,1);
    positionSlider.setRange(0,1);
    speedSlider.setRange(0,10);
}

void DeckGUI::buttonsInitialization() {
    for(const auto& button: allButtons){
        addAndMakeVisible(*button);
        button->addListener(this);
    }
}

void DeckGUI::fastForwardButtonClicked() {
    double newPosition = player->getPosition() + 3.0f;
    if(newPosition > player->getTrackLength()){
        player->setRelativePosition(1.0f);
    }else{
        player->setPosition(newPosition);
    }
}
void DeckGUI::rewindButtonClicked() {
    double newPosition = player->getPosition() - 3.0f;
    if(newPosition < 0){
        player->setPosition(0);
        player->start();
    }else{
        player->setPosition(newPosition);
    }
}