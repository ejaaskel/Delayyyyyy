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
    setSize(450, 300);
    setResizable(true, true);
    setResizeLimits(450, 300, 1920, 1080);

    /* Delay slider init */
    delayAmount.setSliderStyle(juce::Slider::LinearVertical);
    delayAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    delayAmount.setTextValueSuffix(" s");

    delayLabel.setText("Max delay", juce::dontSendNotification);
    delayLabel.attachToComponent(&delayAmount, false);
    delayLabel.setJustificationType(juce::Justification::centredBottom);

    delayAmount.addListener(this);
    addAndMakeVisible(&delayAmount);
    delayAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "MAXDELAY", delayAmount));

    /* Synced delay slider init (NOT ENABLED YET) */
    /*delayAmountSynced.setSliderStyle(juce::Slider::LinearVertical);
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
    delayAmountSynced.setValue(1.0);*/

    //delayAmountSynced.addListener(this);
    //addChildComponent(&delayAmountSynced);

    /* Echo amount slider init */
    echoAmount.setSliderStyle(juce::Slider::LinearVertical);
    echoAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    echoLabel.setText("Echoes", juce::dontSendNotification);
    echoLabel.attachToComponent(&echoAmount, false);
    echoLabel.setJustificationType(juce::Justification::centredBottom);

    echoAmount.addListener(this);
    addAndMakeVisible(&echoAmount);
    echoAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "ECHOES", echoAmount));

    /* Decay knob init */
    decayAmount.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decayAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    decayAmount.setTextValueSuffix("");

    decayLabel.setText("Decay Rate", juce::dontSendNotification);
    decayLabel.attachToComponent(&decayAmount, false);
    decayLabel.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(&decayAmount);
    decayAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "DECAY", decayAmount));

    /* Ping Pong knob init */
    pingPongAmount.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pingPongAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    pingPongAmount.setTextValueSuffix("%");

    pingPongLabel.setText("Ping Pong", juce::dontSendNotification);
    pingPongLabel.attachToComponent(&pingPongAmount, false);
    pingPongLabel.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(&pingPongAmount);
    pingPongAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "PINGPONG", pingPongAmount));

    /* Wet knob init */
    wetAmount.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    wetAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    wetAmount.setTextValueSuffix("%");

    wetLabel.setText("Wet", juce::dontSendNotification);
    wetLabel.attachToComponent(&wetAmount, false);
    wetLabel.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(&wetAmount);
    wetAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "WET", wetAmount));

    /* BPM sync initialization (NOT ENABLED YET) */
    //bpmSync.setButtonText("BPM Sync");
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
    int marginSize = 4;
    //Top margin is larger to accomodate the label
    int topMarginSize = 20 + marginSize;

    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;

    fb.items.add(juce::FlexItem(delayAmount).withMinWidth(75.0f).withMinHeight(75.0f)
                                            .withAlignSelf(juce::FlexItem::AlignSelf::stretch)
                                            .withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));
    fb.items.add(juce::FlexItem(echoAmount).withMinWidth(75.0f).withMinHeight(75.0f)
                                           .withAlignSelf(juce::FlexItem::AlignSelf::stretch)
                                           .withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));
    fb.items.add(juce::FlexItem(decayAmount).withMinWidth(75.0f).withMinHeight(75.0f)
                                            .withAlignSelf(juce::FlexItem::AlignSelf::flexStart)
                                            .withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));
    fb.items.add(juce::FlexItem(pingPongAmount).withMinWidth(75.0f).withMinHeight(75.0f)
                                               .withAlignSelf(juce::FlexItem::AlignSelf::flexStart)
                                               .withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));
    fb.items.add(juce::FlexItem(wetAmount).withMinWidth(75.0f).withMinHeight(75.0f)
                                          .withAlignSelf(juce::FlexItem::AlignSelf::flexStart)
                                          .withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));

    fb.performLayout(getLocalBounds().toFloat());
}

void DelayyyyyyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &delayAmount || slider == &echoAmount) {
        audioProcessor.setDelayBufferParams();
    }
}