/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayyyyyyAudioProcessorEditor::DelayyyyyyAudioProcessorEditor (DelayyyyyyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    /* Delay slider init */
    delayAmount.setSliderStyle(juce::Slider::LinearVertical);
    //TODO: Investigate if this setRange is required anymore now that value trees are used
    delayAmount.setRange(0.0, 5.0, 0.001);
    delayAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    delayAmount.setTextValueSuffix(" s");
    delayAmount.setSkewFactor(0.4);
    delayAmount.setValue(1.0);

    delayLabel.setText("Max delay", juce::dontSendNotification);
    delayLabel.attachToComponent(&delayAmount, false);

    delayAmount.addListener(this);
    addAndMakeVisible(&delayAmount);
    delayAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "MAXDELAY", delayAmount));

    /* Synced delay slider init (NOT ENABLED YET) */
    delayAmountSynced.setSliderStyle(juce::Slider::LinearVertical);
    delayAmountSynced.setRange(0.0, 16, 1.0);
    delayAmountSynced.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    delayAmountSynced.textFromValueFunction = [](double val) {
        switch ((int)val) {
        case 0:
            return "1/64";
        case 1:
            return "1/32";
        case 2:
            return "1/16";
        case 3:
            return "1/8";
        case 4:
            return "1/4";
        case 5:
            return "1/2";
        case 6:
            return "1";
        }
        return "";
    };
    delayAmountSynced.setValue(1.0);

    //delayAmountSynced.addListener(this);
    //addChildComponent(&delayAmountSynced);

    /* Echo amount slider init */
    echoAmount.setSliderStyle(juce::Slider::LinearVertical);
    echoAmount.setRange(0.0, 8.0, 1.0);
    echoAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    echoAmount.setValue(3.0);

    echoLabel.setText("Echoes", juce::dontSendNotification);
    echoLabel.attachToComponent(&echoAmount, false);

    echoAmount.addListener(this);
    addAndMakeVisible(&echoAmount);
    echoAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "ECHOES", echoAmount));

    /* Decay knob init */
    decayAmount.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decayAmount.setRange(0.0, 100.0, 0.1);
    decayAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    decayAmount.setTextValueSuffix("");
    decayAmount.setValue(30.0);

    decayLabel.setText("Decay Rate", juce::dontSendNotification);
    decayLabel.attachToComponent(&decayAmount, false);

    addAndMakeVisible(&decayAmount);
    decayAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "DECAY", decayAmount));

    /* Ping Pong knob init */
    pingPongAmount.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pingPongAmount.setRange(0.0, 100.0, 0.1);
    pingPongAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    pingPongAmount.setTextValueSuffix("%");
    pingPongAmount.setValue(0.0);

    pingPongLabel.setText("Ping Pong", juce::dontSendNotification);
    pingPongLabel.attachToComponent(&pingPongAmount, false);

    addAndMakeVisible(&pingPongAmount);
    pingPongAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "PINGPONG", pingPongAmount));

    /* Wet knob init */
    wetAmount.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    wetAmount.setRange(0.0, 100.0, 0.1);
    wetAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    wetAmount.setTextValueSuffix("%");
    wetAmount.setValue(100.0);

    wetLabel.setText("Wet", juce::dontSendNotification);
    wetLabel.attachToComponent(&wetAmount, false);
    wetLabel.setJustificationType(juce::Justification::centredTop);

    addAndMakeVisible(&wetAmount);
    wetAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "WET", wetAmount));

    /* BPM sync initialization (NOT ENABLED YET) */
    bpmSync.setButtonText("BPM Sync");
    //addAndMakeVisible(&bpmSync);
}

DelayyyyyyAudioProcessorEditor::~DelayyyyyyAudioProcessorEditor()
{
}

//==============================================================================
void DelayyyyyyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void DelayyyyyyAudioProcessorEditor::resized()
{
    // TODO: Make UI scalable
    delayAmount.setBounds(40, 40, 60, getHeight() - 60);
    delayAmountSynced.setBounds(40, 40, 60, getHeight() - 60);

    echoAmount.setBounds(130, 40, 60, getHeight() - 60);

    decayAmount.setBounds(200, 40, 60, 60);
    pingPongAmount.setBounds(260, 40, 60, 60);

    wetAmount.setBounds(320, getHeight() - 160, 60, 60);

    bpmSync.setBounds(20, 10, 60, 20);
}

void DelayyyyyyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &delayAmount || slider == &echoAmount) {
        audioProcessor.setDelayBufferParams();
    }
}