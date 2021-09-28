/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayBuffer.h"

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

    float getSyncedDelay(int index);
    void setDelayBufferParams();

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState* getParameters(void);

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;

    std::vector<DelayBuffer> delayBuffers;

    double currentSampleRate = 44100;

    float prevDelayValue = -1.0f;
    float prevSyncedDelayValue = -1.0f;
    float prevEchoValue = -1.0f;
    float prevBpmSyncValue = -1.0f;

    bool bpmSync = false;
    float bpm = 120.0f;

    std::atomic<float>* delayParameter = nullptr;
    std::atomic<float>* syncedDelayParameter = nullptr;
    std::atomic<float>* echoParameter = nullptr;
    std::atomic<float>* decayParameter = nullptr;
    std::atomic<float>* pingPongParameter = nullptr;
    std::atomic<float>* wetParameter = nullptr;
    std::atomic<float>* bpmSyncParameter = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayyyyyyAudioProcessor)
};
