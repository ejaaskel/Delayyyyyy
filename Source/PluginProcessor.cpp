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
                       )
#endif
{
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

//TODO: Slightly misleading name. This sets only the delay buffers, not all params
void DelayyyyyyAudioProcessor::setDelayParams() {
    for (int i = bufferAmount - 1; i >= 0; i = i - 1) {
        DelayBuffer newDelayBuffer;

        //TODO: Delay buffer doesn't really have to be this big for all of the buffers
        // The earlier the buffer plays, the shorter this can be (saving some memory)
        newDelayBuffer.setDelayLineParameters(getTotalNumInputChannels(), delayBufferLength);

        int delayInSamples = (int)(delayLength * currentSampleRate) / juce::jmax(1, 2 * i);
        newDelayBuffer.setDelayWritePosition(delayInSamples);

        delayBuffers.insert(delayBuffers.begin(), newDelayBuffer);
    }

    delayBuffers.resize(bufferAmount);
}

//==============================================================================
void DelayyyyyyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    

    delayBufferLength = (int)(BUFFER_MAX_LEN_SEC * sampleRate);
    currentSampleRate = sampleRate;

    setDelayParams();
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
    int n, m, drp, dwp = 0;
    
    //Create copy of input so that we don't modify input during processing
    juce::AudioBuffer<float> newOutputBuffer;
    newOutputBuffer.makeCopyOf(buffer);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //TODO: This could use some optimization & refactoring for sure
        for (m = 0; m < bufferAmount; m++) {
            auto* channelData = buffer.getReadPointer(channel);
            auto* newOutputBufferChannelData = newOutputBuffer.getWritePointer(channel);

            //When changing delayLine amount from GUI, delayLine accesses can occasionally be out-of-bounds if not careful
            if (m >= delayBuffers.size()) {
                break;
            }
            auto* delayLineData = delayBuffers[m].getDelayLineWritePointer(channel);
            drp = delayBuffers[m].getDelayReadPosition();
            dwp = delayBuffers[m].getDelayWritePosition();
            for (n = 0; n < buffer.getNumSamples(); n++) {
                float inputSample = channelData[n];
                float delaySample = delayLineData[drp];

                float decayMultiplier = juce::jmin(1.0f, (decayAmount * (((float)m + 1.0f) / (float)bufferAmount)) * ((float)bufferAmount * 0.7f));

                double pingPongMultiplier = 1.0;
                if (pingPongAmount != 0.0) {
                    //If odd buffer, pan one side
                    if (m % 2 == 1) {
                        if (channel % 2 == 1) {
                            pingPongMultiplier = 1 - pingPongAmount;
                        }
                    }
                    //If even buffer, pan other side
                    else {
                        if (channel % 2 == 0) {
                            pingPongMultiplier = 1 - pingPongAmount;
                        }
                    }
                }
                //New sample to delay buffer (i.e. new input with multipliers written to some distance in future)
                delayLineData[dwp] = (float)(inputSample * decayMultiplier * pingPongMultiplier * wetAmount);

                drp = (drp + 1) % delayBufferLength;
                dwp = (dwp + 1) % delayBufferLength;

                //Add previous delay signal to the new input buffer
                newOutputBufferChannelData[n] += delaySample;
            }
        }
    }

    //Update the read & write positions for the next block
    for (m = 0; m < bufferAmount; m++) {
        if (m >= delayBuffers.size()) {
            break;
        }
        drp = delayBuffers[m].getDelayReadPosition();
        dwp = delayBuffers[m].getDelayWritePosition();

        delayBuffers[m].setDelayReadPosition((drp + buffer.getNumSamples()) % delayBufferLength);
        delayBuffers[m].setDelayWritePosition((dwp + buffer.getNumSamples()) % delayBufferLength);
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

void DelayyyyyyAudioProcessor::setDelayLength(float delay) {
    delayLength = delay;
    setDelayParams();
}

void DelayyyyyyAudioProcessor::setEchoAmount(int echo) {
    bufferAmount = echo;
    setDelayParams();
}

void DelayyyyyyAudioProcessor::setDecayAmount(float decay) {
    decayAmount = decay;
}

void DelayyyyyyAudioProcessor::setPingPongDelay(double pingPong) {
    pingPongAmount = pingPong;
}

void DelayyyyyyAudioProcessor::setWet(float wet) {
    wetAmount = wet;
}