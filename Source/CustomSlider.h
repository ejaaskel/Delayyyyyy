
#pragma once
#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

using namespace juce;

class CustomSlider : public Slider
{
public:
    CustomSlider();
    ~CustomSlider();

private:
    CustomLookAndFeel customLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomSlider)
};