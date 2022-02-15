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
class LookAndFeel  : public juce::Component
{
public:
    LookAndFeel();
    ~LookAndFeel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeel)
};
