/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayBuffer.h"

//TODO: 10 seconds is a bit much, considering the fact the maximum unsynced delay time is 5 seconds (5 seconds defined in slider
// should consider using a define for it as well). Perhaps this should  be variable, depending on the current maximum delay size?
#define BUFFER_MAX_LEN_SEC 10

//==============================================================================
/**
*/
class DelayyyyyyAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DelayyyyyyAudioProcessor();
    ~DelayyyyyyAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setDelayLength(float delay);
    void setEchoAmount(int echo);
    void setDecayAmount(float decay);
    void setPingPongDelay(double pingPong);
    void setWet(float wet);

private:
    //==============================================================================
    std::vector<DelayBuffer> delayBuffers;

    double currentSampleRate = 44100;

    //TODO: Buffer amount and echo amount are used to describe this same thing, make it more consistent
    int bufferAmount = 3;
    float delayLength = 1.0f;

    float decayAmount = 0.3f;
    //Need double accuracy for a zero comparison in code
    double pingPongAmount = 0.0;

    float wetAmount = 1.0f;

    void setDelayBufferParams();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayyyyyyAudioProcessor)
};
