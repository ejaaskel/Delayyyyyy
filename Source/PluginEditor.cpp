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
    setSize(450, 350);
    setResizable(true, true);
    setResizeLimits(450, 350, 1920, 1080);

    /* Header plugin title */
    pluginTitle.setText(JucePlugin_Name, juce::dontSendNotification);
    pluginTitle.setJustificationType(juce::Justification::centredLeft);
    juce::Font titleFont = pluginTitle.getFont();
    titleFont.setHeight(20);
    titleFont.setBold(true);
    pluginTitle.setFont(titleFont);
    addAndMakeVisible(pluginTitle);

    /* Header plugin credits */
    pluginCredits.setText("Made by Surku\n@surkumusic", juce::dontSendNotification);
    pluginCredits.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(pluginCredits);

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

    /* Version info*/
    versionLabel.setText("Version:\n" + juce::String(JucePlugin_VersionString), juce::dontSendNotification);
    versionLabel.setJustificationType(juce::Justification::bottomRight);
    versionLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::grey);
    addAndMakeVisible(versionLabel);

    /* Delay modes */
    delayModeLabel.setText("Delay Mode", juce::dontSendNotification);
    delayModeLabel.setJustificationType(juce::Justification::centredBottom);
    addAndMakeVisible(delayModeLabel);

    expandModeButton.onClick = [this] { updateToggleState(&expandModeButton, "Expand");   };
    expandModeButton.setRadioGroupId(DelayModeButtons);
    addAndMakeVisible(expandModeButton);

    evenModeButton.onClick = [this] { updateToggleState(&evenModeButton, "Even"); };
    evenModeButton.setRadioGroupId(DelayModeButtons);
    addAndMakeVisible(evenModeButton);

    shrinkModeButton.onClick = [this] { updateToggleState(&shrinkModeButton, "Shrink"); };
    shrinkModeButton.setRadioGroupId(DelayModeButtons);
    addAndMakeVisible(shrinkModeButton);
}

DelayyyyyyAudioProcessorEditor::~DelayyyyyyAudioProcessorEditor()
{
}

//==============================================================================
void DelayyyyyyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);

    g.setColour(Colours::azure);
    g.fillRect(headerSeparator);

    /* Active delay mode toggle set (required when loading plugin) */
    switch (audioProcessor.getDelayMode()) {
        case DelayyyyyyAudioProcessor::DelayModes::EXPAND:
            expandModeButton.setToggleState(true, juce::sendNotificationSync);
            break;
        case DelayyyyyyAudioProcessor::DelayModes::EVEN:
            evenModeButton.setToggleState(true, juce::sendNotificationSync);
            break;
        case DelayyyyyyAudioProcessor::DelayModes::SHRINK:
            shrinkModeButton.setToggleState(true, juce::sendNotificationSync);
            break;
    }
}

void DelayyyyyyAudioProcessorEditor::resized()
{
    int headerSize = 40;
    int marginSize = 4;
    //Top margin is larger to accomodate the label
    int topMarginSize = 20 + marginSize;

    /*Header*/
    Rectangle<int> area = getLocalBounds();
    int headerFooterHeight = headerSize;
    Rectangle<int> header = area.removeFromTop(headerFooterHeight);
    pluginTitle.setBounds(header.removeFromLeft(header.getWidth() / 2));
    pluginCredits.setBounds(header);
    headerSeparator = area.removeFromTop(3);

    /*FlexBox for the actual content*/
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;

    /*Delay amount slider + BPM sync button*/
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

    /*Echo amount slider*/
    juce::FlexBox echoBox;
    echoBox.flexWrap = juce::FlexBox::Wrap::wrap;
    echoBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    echoBox.items.add(juce::FlexItem(echoAmount).withMinWidth(75.0f).withMinHeight(200.0f));
    //Add an empty item to level the bottom with the delay slider
    echoBox.items.add(juce::FlexItem().withMinWidth(75.0f).withMinHeight(15.0f));
    fb.items.add(juce::FlexItem(echoBox).withMinWidth(75.0f).withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));

    /*Decay knob*/
    juce::FlexBox decayBox;
    decayBox.flexWrap = juce::FlexBox::Wrap::wrap;
    decayBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    decayBox.flexDirection = juce::FlexBox::Direction::column;
    decayBox.items.add(juce::FlexItem(decayAmount).withMinWidth(75.0f).withMinHeight(75.0f).withAlignSelf(juce::FlexItem::AlignSelf::flexStart));
    decayBox.items.add(juce::FlexItem().withMinWidth(75.0f).withMinHeight(50.0f));
    decayBox.items.add(juce::FlexItem(expandModeButton).withMinWidth(75.0f).withMinHeight(50.0f).withAlignSelf(juce::FlexItem::AlignSelf::flexStart));
    fb.items.add(juce::FlexItem(decayBox).withMinWidth(75.0f).withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));

    /*Ping Pong knob*/
    juce::FlexBox pingPongBox;
    pingPongBox.flexWrap = juce::FlexBox::Wrap::wrap;
    pingPongBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    pingPongBox.flexDirection = juce::FlexBox::Direction::column;
    pingPongBox.items.add(juce::FlexItem(pingPongAmount).withMinWidth(75.0f).withMinHeight(75.0f).withAlignSelf(juce::FlexItem::AlignSelf::flexStart));
    pingPongBox.items.add(juce::FlexItem(delayModeLabel).withMinWidth(75.0f).withMinHeight(50.0f).withAlignSelf(juce::FlexItem::AlignSelf::flexStart));
    pingPongBox.items.add(juce::FlexItem(evenModeButton).withMinWidth(75.0f).withMinHeight(50.0f).withAlignSelf(juce::FlexItem::AlignSelf::flexStart));
    fb.items.add(juce::FlexItem(pingPongBox).withMinWidth(75.0f).withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));

    /*FlexBox for wet knob and version label*/
    juce::FlexBox wetBox;
    wetBox.flexWrap = juce::FlexBox::Wrap::wrap;
    wetBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    wetBox.flexDirection = juce::FlexBox::Direction::column;
    wetBox.items.add(juce::FlexItem(wetAmount).withMinWidth(75.0f).withMinHeight(75.0f).withAlignSelf(juce::FlexItem::AlignSelf::flexStart));
    wetBox.items.add(juce::FlexItem().withMinWidth(75.0f).withMinHeight(50.0f));
    wetBox.items.add(juce::FlexItem(shrinkModeButton).withMinWidth(75.0f).withMinHeight(50.0f).withAlignSelf(juce::FlexItem::AlignSelf::flexStart));
    wetBox.items.add(juce::FlexItem(versionLabel).withMinWidth(75.0f).withMinHeight(75.0f).withAlignSelf(juce::FlexItem::AlignSelf::flexStart).withFlex(1));
    fb.items.add(juce::FlexItem(wetBox).withMinWidth(75.0f).withMargin(juce::FlexItem::Margin(topMarginSize, marginSize, marginSize, marginSize)));

    fb.performLayout(area.toFloat());
}

void DelayyyyyyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &delayAmount || slider == &syncedDelayAmount || slider == &echoAmount) {
        audioProcessor.notifyThread();
    }
}

void DelayyyyyyAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    resized();
    audioProcessor.notifyThread();
}

void DelayyyyyyAudioProcessorEditor::updateToggleState(juce::ToggleButton* button, juce::String name)
{
    bool state = button->getToggleState();

    if (state) {
        if (button == &expandModeButton) {
            audioProcessor.setDelayMode(DelayyyyyyAudioProcessor::DelayModes::EXPAND);
        }
        else if (button == &evenModeButton) {
            audioProcessor.setDelayMode(DelayyyyyyAudioProcessor::DelayModes::EVEN);
        }
        else if (button == &shrinkModeButton) {
            audioProcessor.setDelayMode(DelayyyyyyAudioProcessor::DelayModes::SHRINK);
        }
        audioProcessor.notifyThread();
    }
}