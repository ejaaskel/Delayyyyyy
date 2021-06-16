/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayyyyyyAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener, private juce::Button::Listener
{
public:
    DelayyyyyyAudioProcessorEditor (DelayyyyyyAudioProcessor&);
    ~DelayyyyyyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* slider) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayyyyyyAudioProcessor& audioProcessor;

    juce::Slider delayAmount;
    juce::Label delayLabel;
    juce::Slider delayAmountSynced;

    juce::Slider bufferAmount;
    juce::Label bufferLabel;

    juce::Label  feedbackLabel;
    juce::Slider feedbackAmount;
    juce::Label  pingPongLabel;
    juce::Slider pingPongAmount;

    juce::Label  wetLabel;
    juce::Slider wetAmount;

    juce::ToggleButton bpmSync;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayyyyyyAudioProcessorEditor)
};
