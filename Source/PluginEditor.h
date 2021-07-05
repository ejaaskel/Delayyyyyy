/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class DelayyyyyyAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    DelayyyyyyAudioProcessorEditor (DelayyyyyyAudioProcessor&);
    ~DelayyyyyyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayyyyyyAudioProcessor& audioProcessor;

    juce::Slider delayAmount;
    juce::Label delayLabel;
    std::unique_ptr<SliderAttachment> delayAttachment;
    juce::Slider delayAmountSynced;

    juce::Slider echoAmount;
    std::unique_ptr<SliderAttachment> echoAttachment;
    juce::Label echoLabel;

    juce::Label  decayLabel;
    juce::Slider decayAmount;
    std::unique_ptr<SliderAttachment> decayAttachment;
    juce::Label  pingPongLabel;
    juce::Slider pingPongAmount;
    std::unique_ptr<SliderAttachment> pingPongAttachment;

    juce::Label  wetLabel;
    juce::Slider wetAmount;
    std::unique_ptr<SliderAttachment> wetAttachment;

    juce::ToggleButton bpmSync;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayyyyyyAudioProcessorEditor)
};
