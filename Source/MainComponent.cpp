#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 900);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();

    deckGUIGroup.addDeckAndPlayer(&deckGUI1);
    deckGUIGroup.addDeckAndPlayer(&deckGUI2);

//
//    playButton.addListener(this);
//    stopButton.addListener(this);
//    loadButton.addListener(this);
//    volumeSlider.addListener(this);
//    gainSlider.addListener(this);
//    speedSlider.addListener(this);
//    positionSlider.addListener(this);
//
//    gainSlider.setRange(0,1);
//    positionSlider.setRange(0,1);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    isPlaying = true;

    gain = 0.5;

    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected,sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    if(!isPlaying){
        bufferToFill.clearActiveBufferRegion();
    }else{
        //transportSource.getNextAudioBlock(bufferToFill);
        mixerSource.getNextAudioBlock(bufferToFill);

//        float* leftChannel = bufferToFill.buffer->getWritePointer(0,
//                                                                  bufferToFill.startSample);
//        float* rightChannel = bufferToFill.buffer->getWritePointer(1,
//                                                                   bufferToFill.startSample);
//        for(int i = 0; i < bufferToFill.numSamples; ++i){
//            //float sample = fmod(phase, 0.2f);
//            float sample = sin(phase) * gain * 0.125;
//            //dynamicPhase += 0.0000005f;
//            leftChannel[i] = sample;
//            rightChannel[i] = sample;
//
//            phase += dynamicPhase;
//        }
    }


    //bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
//
//    double rowHeight = getHeight()/10;
//    playButton.setBounds(0,0,getWidth(), rowHeight);
//    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);
//    loadButton.setBounds(0, rowHeight*2, getWidth(), rowHeight);
//    volumeSlider.setBounds(0, rowHeight*4, getWidth(), rowHeight);
//    gainSlider.setBounds(0, rowHeight*5, getWidth(), rowHeight);
//    speedSlider.setBounds(0, rowHeight*6, getWidth(), rowHeight);
//    positionSlider.setBounds(0, rowHeight*7, getWidth(), rowHeight);

    deckGUI1.setBounds(0,0,getWidth()/2, getHeight()/2);
    deckGUI2.setBounds(getWidth()/2,0, getWidth()/2, getHeight()/2);

    playlistComponent.setBounds(0, getHeight()/2, getWidth(), getHeight()/2);
}

//void MainComponent::buttonClicked(juce::Button * button) {
////    if(button == &playButton){
////        isPlaying = true;
////        player1.start();
////    }else if(button == &stopButton){
////        isPlaying = false;
////        player1.stop();
////    }else if(button == &loadButton){
////        juce::FileChooser chooser{"Select a audio file to play..."};
////        if(chooser.browseForFileToOpen()){
////
////            player1.loadURL(juce::URL{chooser.getResult()});
////        }
////    }
//}
//
//void MainComponent::sliderValueChanged(juce::Slider *slider) {
////    double value = slider->getValue();
////    if(slider == &volumeSlider){
////
////    }else if(slider == &gainSlider){
////        gain = value;
////        player1.setGain(value);
////    }else if(slider == &speedSlider){
////        player1.setSpeed(value);
////        //resampleSource.setResamplingRatio(slider->getValue());
////    }else if(slider == &positionSlider){
////        player1.setRelativePosition(value);
////        //transportSource.setPosition(positionSlider.getValue());
////    }
//}




