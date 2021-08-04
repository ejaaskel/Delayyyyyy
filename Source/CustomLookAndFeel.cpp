
#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel() : LookAndFeel_V4()
{

}

void CustomLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    const float diameter = jmin(width, height);
    const float radius = diameter / 2;
    const float centerX = x + width / 2;
    const float centerY = y + height / 2;
    const float rx = centerX - radius;
    const float ry = centerY - radius;
    const float angle = sliderPos * (rotaryEndAngle - rotaryStartAngle);

    //Color background of the knob black
    Rectangle<float> knobArea(rx, ry, diameter, diameter);
    g.setColour(Colours::black);
    g.fillEllipse(knobArea);

    //Add a white line to mark the knob drag area
    Path dial;
    //5 is used to draw the arc slightly in
    int offsetAmount = 5;
    dial.addCentredArc(rx + radius, ry + radius, radius - offsetAmount, radius - offsetAmount, 0, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(Colours::white);
    g.strokePath(dial, PathStrokeType(2.0f));

    //Get the first point of the mark above, needed for the indicator
    Path::Iterator i(dial);
    //Skip the initial 0, 0 point
    i.next();

    //Paint the white indicator circle
    Path indicator;
    int indicatorCircleSize = 10;
    //Subtract half of the indicatorCircleSize to center the indicator correctly
    indicator.addEllipse(i.x1 - indicatorCircleSize / 2, i.y1 - indicatorCircleSize / 2, indicatorCircleSize, indicatorCircleSize);
    g.fillPath(indicator, AffineTransform::rotation(angle, rx + radius, ry + radius));
}

void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider)
{

}

void CustomLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider)
{

}