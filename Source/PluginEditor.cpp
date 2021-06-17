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
    delayAmount.setRange(0.0, 5.0, 0.001);
    delayAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    delayAmount.setTextValueSuffix(" s");
    delayAmount.setSkewFactor(0.4);
    delayAmount.setValue(1.0);

    delayLabel.setText("Max delay", juce::dontSendNotification);
    delayLabel.attachToComponent(&delayAmount, false);

    delayAmount.addListener(this);
    addAndMakeVisible(&delayAmount);

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

    delayAmountSynced.addListener(this);
    //addChildComponent(&delayAmountSynced);

    /* Buffer amount slider init */
    bufferAmount.setSliderStyle(juce::Slider::LinearVertical);
    bufferAmount.setRange(0.0, 8.0, 1.0);
    bufferAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    bufferAmount.setValue(3.0);

    bufferLabel.setText("Echoes", juce::dontSendNotification);
    bufferLabel.attachToComponent(&bufferAmount, false);

    bufferAmount.addListener(this);
    addAndMakeVisible(&bufferAmount);

    /* Decay knob init */
    decayAmount.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decayAmount.setRange(0.0, 100.0, 0.1);
    decayAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    decayAmount.setTextValueSuffix("");
    decayAmount.setValue(30.0);

    decayLabel.setText("Decay Rate", juce::dontSendNotification);
    decayLabel.attachToComponent(&decayAmount, false);

    decayAmount.addListener(this);
    addAndMakeVisible(&decayAmount);

    /* Ping Pong knob init */
    pingPongAmount.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pingPongAmount.setRange(0.0, 100.0, 0.1);
    pingPongAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    pingPongAmount.setTextValueSuffix("%");
    pingPongAmount.setValue(0.0);

    pingPongLabel.setText("Ping Pong", juce::dontSendNotification);
    pingPongLabel.attachToComponent(&pingPongAmount, false);

    pingPongAmount.addListener(this);
    addAndMakeVisible(&pingPongAmount);

    /* Wet knob init */
    wetAmount.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    wetAmount.setRange(0.0, 100.0, 0.1);
    wetAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    wetAmount.setTextValueSuffix("%");
    wetAmount.setValue(100.0);

    wetLabel.setText("Wet", juce::dontSendNotification);
    wetLabel.attachToComponent(&wetAmount, false);
    wetLabel.setJustificationType(juce::Justification::centredTop);

    wetAmount.addListener(this);
    addAndMakeVisible(&wetAmount);

    /* BPM sync initialization (NOT ENABLED YET) */
    bpmSync.setButtonText("BPM Sync");
    bpmSync.addListener(this);
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

    bufferAmount.setBounds(130, 40, 60, getHeight() - 60);

    decayAmount.setBounds(200, 40, 60, 60);
    pingPongAmount.setBounds(260, 40, 60, 60);

    wetAmount.setBounds(320, getHeight() - 160, 60, 60);

    bpmSync.setBounds(20, 10, 60, 20);
}

void DelayyyyyyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //TODO: Especially with delayLength option there's a lot of performance hit
    // Perhaps don't call it with every slide of the slider

    //TODO: This really really REALLY should be done with value trees
    if (slider == &delayAmount) {
        audioProcessor.setDelayLength((float)delayAmount.getValue());
    }
    else if (slider == &bufferAmount) {
        audioProcessor.setEchoAmount((int)bufferAmount.getValue());
    }
    else if (slider == &decayAmount) {
        //Higher decay rate means lower multiplier to the signal when processing, so subtract the value of slider from 1
        audioProcessor.setDecayAmount(1.0f - (float)decayAmount.getValue() / 100.0f);
    }
    else if (slider == &pingPongAmount) {
        audioProcessor.setPingPongDelay(pingPongAmount.getValue() / 100.0);
    }
    else if (slider == &wetAmount) {
        audioProcessor.setWet((float)wetAmount.getValue() / 100.0f);
    }
}

void DelayyyyyyAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    delayAmount.setVisible(!delayAmount.isVisible());
    delayAmountSynced.setVisible(!delayAmountSynced.isVisible());
}