/*
  ==============================================================================

    LookAndFeel.h
    Created: 15 Feb 2022 10:04:49am
    Author:  matt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LookAndFeel  : public juce::LookAndFeel_V4
{
public:
    LookAndFeel();
    juce::Label* createSliderTextBox (juce::Slider& slider) override;


};
