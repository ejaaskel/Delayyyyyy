
#include "DelayBuffer.h"

DelayBuffer::DelayBuffer() {
    delayLine.clear();
    delayReadPosition = 0;
    delayWritePosition = 0;
}

DelayBuffer::~DelayBuffer()
{
}

void DelayBuffer::setDelayLineParameters(int numChannels, int numSamples) {
    delayLine.setSize(numChannels, numSamples);
    delayLine.clear();
}

float* DelayBuffer::getDelayLineWritePointer(int channel) {
    return delayLine.getWritePointer(channel);
}

void DelayBuffer::setDelayReadPosition(int delayReadPosition) {
    this->delayReadPosition = delayReadPosition;
}

int DelayBuffer::getDelayReadPosition() {
    return delayReadPosition;
}

void DelayBuffer::setDelayWritePosition(int delayWritePosition) {
    this->delayWritePosition = delayWritePosition;
}

int DelayBuffer::getDelayWritePosition() {
    return delayWritePosition;
}