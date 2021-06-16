/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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
    void setFeedbackAmount(float feedback);
    void setPingPongDelay(double pingPong);
    void setWet(float wet);

private:
    //==============================================================================

    //TODO: instead of having these in three separate vectors, they could be refactored into one class
    // This way only one vector would be needed to avoid them getting mismatched
    std::vector<juce::AudioBuffer<float>> delayLines;
    std::vector<int> delayReadPositions;
    std::vector<int> delayWritePositions;

    double currentSampleRate = 44100;

    int delayBufferLength = (int)currentSampleRate * 10;
    //TODO: Buffer amount and echo amount are used to describe this same thing, make it more consistent
    int bufferAmount = 3;
    float delayLength = 1.0f;

    float feedbackAmount = 0.7f;
    //Need double accuracy for a zero comparison in code
    double pingPongAmount = 0.0;

    float wetAmount = 1.0f;

    void setDelayParams();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayyyyyyAudioProcessor)
};
