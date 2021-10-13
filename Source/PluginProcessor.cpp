/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayyyyyyAudioProcessor::DelayyyyyyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters(*this, nullptr, juce::Identifier("VALUETREE"),
        {
            std::make_unique<juce::AudioParameterFloat>("MAXDELAY",        // parameterID
                                                        "Max Delay",       // parameter name
                                                        juce::NormalisableRange<float>(0.0f, 5.0f, 0.001f, 0.5f, false), //NormalisableRange for adding skew
                                                        1.0f),             // default value
            std::make_unique<juce::AudioParameterInt>("SYNCEDMAXDELAY",
                                                      "Synced Max Delay",
                                                      0,
                                                      9,
                                                      3),
            std::make_unique<juce::AudioParameterInt>("ECHOES",
                                                      "Echoes",
                                                      0,
                                                      8,
                                                      3),
            std::make_unique<juce::AudioParameterFloat>("DECAY",
                                                        "Decay Rate",
                                                        0.0f,
                                                        100.0f,
                                                        30.0f),
            std::make_unique<juce::AudioParameterFloat>("PINGPONG",
                                                        "Ping Pong",
                                                        0.0f,
                                                        100.0f,
                                                        0.0f),
            std::make_unique<juce::AudioParameterFloat>("WET",
                                                        "Wet",
                                                        0.0f,
                                                        100.0f,
                                                        100.0f),
            std::make_unique<juce::AudioParameterBool>("BPMSYNC",
                                                       "BPM Sync",
                                                       false),
        })
{
    delayParameter = parameters.getRawParameterValue("MAXDELAY");
    syncedDelayParameter = parameters.getRawParameterValue("SYNCEDMAXDELAY");
    echoParameter = parameters.getRawParameterValue("ECHOES");
    decayParameter = parameters.getRawParameterValue("DECAY");
    pingPongParameter = parameters.getRawParameterValue("PINGPONG");
    wetParameter = parameters.getRawParameterValue("WET");
    bpmSyncParameter = parameters.getRawParameterValue("BPMSYNC");
}

DelayyyyyyAudioProcessor::~DelayyyyyyAudioProcessor()
{
}

//==============================================================================
const juce::String DelayyyyyyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayyyyyyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayyyyyyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayyyyyyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayyyyyyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayyyyyyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayyyyyyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayyyyyyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayyyyyyAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayyyyyyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

float DelayyyyyyAudioProcessor::getSyncedDelay(int index) {
    switch (index) {
    case 0:
        return 1.0f/32.0f;
    case 1:
        return 1.0f/16.0f;
    case 2:
        return 1.0f/8.0f;
    case 3:
        return 1.0f/4.0f;
    case 4:
        return 1.0f/2.0f;
    case 5:
        return 1;
    case 6:
        return 2;
    case 7:
        return 4;
    case 8:
        return 8;
    case 9:
        return 16;
    default:
        return -1;

    }
}

void DelayyyyyyAudioProcessor::setDelayBufferParams() {
    if (*delayParameter == prevDelayValue &&
        *echoParameter == prevEchoValue &&
        *syncedDelayParameter == prevSyncedDelayValue &&
        *bpmSyncParameter == prevBpmSyncValue) {
        return;
    }
    else {
        prevDelayValue = *delayParameter;
        prevEchoValue = *echoParameter;
        prevSyncedDelayValue = *syncedDelayParameter;
        prevBpmSyncValue = *bpmSyncParameter;
    }

    std::vector<DelayBuffer> newDelayBuffers;
    for (int i = (int)*echoParameter - 1; i >= 0; i = i - 1) {
        int delayInSamples = 0;

        if (!*bpmSyncParameter) {
            int divider = juce::jmax(1, 2 * i);
            delayInSamples = (int)(*delayParameter * currentSampleRate) / divider;
        }
        else {
            //Magic number 60 comes from the amount of seconds in a minute
            //Magic number 4 comes from the time signature 4/4
            //Other time signatures are out of scope for this project (PRs welcome of course :))
            float durationOfBar = 60.0f / bpm * 4.0f;

            float syncedDelay = getSyncedDelay(*syncedDelayParameter - i);
            if (syncedDelay != -1) {
                delayInSamples = ((float)durationOfBar * (float)currentSampleRate) * syncedDelay;
            }
            else {
                continue;
            }
        }
        DelayBuffer newDelayBuffer;

        newDelayBuffer.setDelayLineParameters(getTotalNumInputChannels(), delayInSamples + 1);
        newDelayBuffer.setDelayWritePosition(delayInSamples);

        newDelayBuffers.insert(newDelayBuffers.begin(), newDelayBuffer);
    }
    delayBuffers = newDelayBuffers;
}

//==============================================================================
void DelayyyyyyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    currentSampleRate = sampleRate;
    setDelayBufferParams();
}

void DelayyyyyyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayyyyyyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayyyyyyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    int n, m, drp, dwp, curDelayBufferSize = 0;
    
    //Create copy of input so that we don't modify input during processing
    juce::AudioBuffer<float> newOutputBuffer;
    newOutputBuffer.makeCopyOf(buffer);

    //Higher decay rate means lower multiplier to the signal when processing, so subtract the value of slider from 1
    float currentDecay = 1.0f - (*decayParameter / 100.0f);
    float currentPingPong = *pingPongParameter / 100.0f;
    float currentWet = *wetParameter / 100.0f;

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //TODO: This could use some optimization & refactoring for sure
        for (m = 0; m < *echoParameter; m++) {
            auto* channelData = buffer.getReadPointer(channel);
            auto* newOutputBufferChannelData = newOutputBuffer.getWritePointer(channel);

            //When changing delayLine amount from GUI, delayLine accesses can occasionally be out-of-bounds if not careful
            if (m >= delayBuffers.size()) {
                break;
            }
            auto* delayLineData = delayBuffers[m].getDelayLineWritePointer(channel);
            curDelayBufferSize = delayBuffers[m].getDelayLineSize();
            drp = delayBuffers[m].getDelayReadPosition();
            dwp = delayBuffers[m].getDelayWritePosition();
            for (n = 0; n < buffer.getNumSamples(); n++) {
                float inputSample = channelData[n];
                //TODO: This line occasionally fails when changing delay parameters. Mutexing?
                float delaySample = delayLineData[drp];

                //TODO: Change this into something a bit more sensible...
                float decayMultiplier = juce::jmin(1.0f, (currentDecay * (((float)m + 1.0f) / (float)*echoParameter)) * ((float)*echoParameter * 0.7f));

                double pingPongMultiplier = 1.0;
                if (currentPingPong != 0.0) {
                    //If odd buffer, pan one side
                    if (m % 2 == 1) {
                        if (channel % 2 == 1) {
                            pingPongMultiplier = 1 - currentPingPong;
                        }
                    }
                    //If even buffer, pan other side
                    else {
                        if (channel % 2 == 0) {
                            pingPongMultiplier = 1 - currentPingPong;
                        }
                    }
                }
                //New sample to delay buffer (i.e. new input with multipliers written to some distance in future)
                delayLineData[dwp] = (float)(inputSample * decayMultiplier * pingPongMultiplier * currentWet);

                drp = (drp + 1) % curDelayBufferSize;
                dwp = (dwp + 1) % curDelayBufferSize;

                //Add previous delay signal to the new input buffer
                newOutputBufferChannelData[n] += delaySample;
            }
        }
    }

    //Update the read & write positions for the next block
    for (m = 0; m < *echoParameter; m++) {
        if (m >= delayBuffers.size()) {
            break;
        }
        curDelayBufferSize = delayBuffers[m].getDelayLineSize();
        drp = delayBuffers[m].getDelayReadPosition();
        dwp = delayBuffers[m].getDelayWritePosition();

        delayBuffers[m].setDelayReadPosition((drp + buffer.getNumSamples()) % curDelayBufferSize);
        delayBuffers[m].setDelayWritePosition((dwp + buffer.getNumSamples()) % curDelayBufferSize);
    }

    buffer.copyFrom(0, 0, newOutputBuffer.getReadPointer(0), newOutputBuffer.getNumSamples());
    buffer.copyFrom(1, 0, newOutputBuffer.getReadPointer(1), newOutputBuffer.getNumSamples());
}

//==============================================================================
bool DelayyyyyyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayyyyyyAudioProcessor::createEditor()
{
    return new DelayyyyyyAudioProcessorEditor (*this);
}

//==============================================================================
void DelayyyyyyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayyyyyyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayyyyyyAudioProcessor();
}

juce::AudioProcessorValueTreeState* DelayyyyyyAudioProcessor::getParameters() {
    return &parameters;
}