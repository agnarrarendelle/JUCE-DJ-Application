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
    LookAndFeel()
    {
        setColour (juce::Slider::thumbColourId, juce::Colours::blueviolet);
        //setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::grey);//设置旋钮数值条背景颜色
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgrey);  //设置旋钮填充色


    }
    juce::Label* createSliderTextBox (juce::Slider& slider) override
    {
        juce::Label* l = LookAndFeel_V4::createSliderTextBox (slider);
        l->setColour (juce::Label::outlineColourId, juce::Colours::transparentWhite );
        l->setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite );
        l->setFont(juce::Font(28.0f, juce::Font::bold));
        l-> setColour(juce::Label::textColourId, juce::Colours::white);
        //l->setJustificationType(juce::Justification::horizontallyCentred);
        return l;
    }
//    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
//                               bool, bool isButtonDown) override{
//
//    }

//    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
//                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
//    {
//        auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
//        auto centreX = (float) x + (float) width  * 0.5f;
//        auto centreY = (float) y + (float) height * 0.5f;
//        auto rx = centreX - radius;
//        auto ry = centreY - radius;
//        auto rw = radius * 2.0f;
//        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
//
//        // fill
//        g.setColour (juce::Colours::orange);
//        g.fillEllipse (rx, ry, rw, rw);
//
//        // outline
//        g.setColour (juce::Colours::red);
//        g.drawEllipse (rx, ry, rw, rw, 1.0f);
//
//        juce::Path p;
//        auto pointerLength = radius * 0.33f;
//        auto pointerThickness = 2.0f;
//        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
//        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
//
//        // pointer
//        g.setColour (juce::Colours::yellow);
//        g.fillPath (p);
//    }
};
