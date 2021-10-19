/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomSlider.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

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
    void buttonClicked(juce::Button* button) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayyyyyyAudioProcessor& audioProcessor;

    juce::Label pluginTitle;
    juce::Label pluginCredits;

    juce::Rectangle<int> headerSeparator;

    CustomSlider delayAmount;
    juce::Label delayLabel;
    std::unique_ptr<SliderAttachment> delayAttachment;

    CustomSlider syncedDelayAmount;
    juce::Label syncedDelayLabel;
    std::unique_ptr<SliderAttachment> syncedDelayAttachment;

    CustomSlider echoAmount;
    std::unique_ptr<SliderAttachment> echoAttachment;
    juce::Label echoLabel;

    juce::Label  decayLabel;
    CustomSlider decayAmount;
    std::unique_ptr<SliderAttachment> decayAttachment;
    juce::Label  pingPongLabel;
    CustomSlider pingPongAmount;
    std::unique_ptr<SliderAttachment> pingPongAttachment;

    juce::Label  wetLabel;
    CustomSlider wetAmount;
    std::unique_ptr<SliderAttachment> wetAttachment;

    juce::ToggleButton bpmSync;
    std::unique_ptr<ButtonAttachment> bpmSyncAttachment;

    juce::Label versionLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayyyyyyAudioProcessorEditor)
};
