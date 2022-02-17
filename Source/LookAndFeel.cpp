/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 15 Feb 2022 10:04:49am
    Author:  matt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LookAndFeel.h"

//==============================================================================

LookAndFeel::LookAndFeel() {
    setColour (juce::Slider::thumbColourId, juce::Colours::blueviolet);
    //setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::grey);//设置旋钮数值条背景颜色
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgrey);  //设置旋钮填充色
}

juce::Label *LookAndFeel::createSliderTextBox(juce::Slider &slider) {
    juce::Label* l = LookAndFeel_V4::createSliderTextBox (slider);
    l->setColour (juce::Label::outlineColourId, juce::Colours::transparentWhite );
    l->setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite );
    l->setFont(juce::Font(28.0f, juce::Font::bold));
    l-> setColour(juce::Label::textColourId, juce::Colours::white);
    //l->setJustificationType(juce::Justification::horizontallyCentred);
    return l;
}