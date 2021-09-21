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

    /* Synced delay slider init */
    syncedDelayAmount.setSliderStyle(juce::Slider::LinearVertical);
    syncedDelayAmount.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    syncedDelayLabel.setText("Max delay", juce::dontSendNotification);
    syncedDelayLabel.attachToComponent(&syncedDelayAmount, false);
    syncedDelayLabel.setJustificationType(juce::Justification::centredBottom);

    syncedDelayAmount.addListener(this);
    addAndMakeVisible(&syncedDelayAmount);
    syncedDelayAttachment.reset(new SliderAttachment(*audioProcessor.getParameters(), "SYNCEDMAXDELAY", syncedDelayAmount));

    syncedDelayAmount.textFromValueFunction = [](double val) {
        switch ((int)val) {
        case 0:
            return "1/32";
        case 1:
            return "1/16";
        case 2:
            return "1/8";
        case 3:
            return "1/4";
        case 4:
            return "1/2";
        case 5:
            return "1";
        case 6:
            return "2";
        case 7:
            return "4";
        case 8:
            return "8";
        case 9:
            return "16";
        }
        return "";
    };
    //Required here, because we change the textFromValueFunction after resetting the syncedDelayAttachment.
    //It seems like reset also resets the syncedDelayAttachment, so these are done in a bit odd order
    syncedDelayAmount.updateText();

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

    /* BPM sync initialization */
    bpmSync.setButtonText("BPM Sync");
    addAndMakeVisible(&bpmSync);
    bpmSync.addListener(this);
    bpmSyncAttachment.reset(new ButtonAttachment(*audioProcessor.getParameters(), "BPMSYNC", bpmSync));
}

DelayyyyyyAudioProcessorEditor::~DelayyyyyyAudioProcessorEditor()
{
}

//==============================================================================
void DelayyyyyyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
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

    juce::FlexBox delayBox;
    delayBox.flexWrap = juce::FlexBox::Wrap::wrap;
    delayBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    if (!bpmSync.getToggleState()) {
        delayAmount.setVisible(true);
        syncedDelayAmount.setVisible(false);
        delayBox.items.add(juce::FlexItem(delayAmount).withMinWidth(75.0f).withMinHeight(200.0f));
    }
    else {
        delayAmount.setVisible(false);
        syncedDelayAmount.setVisible(true);
        delayBox.items.add(juce::FlexItem(syncedDelayAmount).withMinWidth(75.0f).withMinHeight(200.0f));
    }
    delayBox.items.add(juce::FlexItem(bpmSync).withMinWidth(75.0f).withMinHeight(15.0f));
    fb.items.add(juce::FlexItem(delayBox).withMinWidth(75.0f).withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));

    juce::FlexBox echoBox;
    echoBox.flexWrap = juce::FlexBox::Wrap::wrap;
    echoBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    echoBox.items.add(juce::FlexItem(echoAmount).withMinWidth(75.0f).withMinHeight(200.0f));
    //Add an empty item to level the bottom with the delay slider
    echoBox.items.add(juce::FlexItem().withMinWidth(75.0f).withMinHeight(15.0f));

    fb.items.add(juce::FlexItem(echoBox).withMinWidth(75.0f).withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));
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
    if (slider == &delayAmount || slider == &syncedDelayAmount || slider == &echoAmount) {
        audioProcessor.setDelayBufferParams();
    }
}

void DelayyyyyyAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    resized();
    //TODO: This should be forced when the button has been clicked, because no value has changed
    //Or add the bpmsyncstatus check to function
    audioProcessor.setDelayBufferParams();
}