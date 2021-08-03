
#include <JuceHeader.h>
#include "CustomSlider.h"

CustomSlider::CustomSlider() : Slider()
{
    setLookAndFeel(&customLookAndFeel);
}

CustomSlider::~CustomSlider()
{
    setLookAndFeel(nullptr);
}