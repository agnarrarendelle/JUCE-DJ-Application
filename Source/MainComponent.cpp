#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (700, 900);

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


    addAndMakeVisible(deckGUIGroup);
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();

    allPlayers.push_back(&player1);
    allPlayers.push_back(&player2);

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
    mixerSource.prepareToPlay(samplesPerBlockExpected,sampleRate);
    for( auto& player:allPlayers){
        player->prepareToPlay(samplesPerBlockExpected,sampleRate);
    }
//    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
//    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);


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
    for(DJAudioPlayer* player:allPlayers){
        player->releaseResources();
    }
//    player1.releaseResources();
//    player2.releaseResources();
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
    deckGUIGroup.setBounds(0,0,getWidth(), getHeight()/2);
    playlistComponent.setBounds(0, getHeight()/2, getWidth(), getHeight()/2);
}






