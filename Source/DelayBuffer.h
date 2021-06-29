
#pragma once

#include <JuceHeader.h>

class DelayBuffer {
public:
    DelayBuffer();
    ~DelayBuffer();

    void setDelayLineParameters(int numChannels, int numSamples);
    float *getDelayLineWritePointer(int channel);
    int getDelayLineSize();

    void setDelayReadPosition(int delayReadPosition);
    int getDelayReadPosition();

    void setDelayWritePosition(int delayWritePosition);
    int getDelayWritePosition();

private:
    juce::AudioBuffer<float> delayLine;
    int delayLineSize;
    int delayReadPosition;
    int delayWritePosition;
};