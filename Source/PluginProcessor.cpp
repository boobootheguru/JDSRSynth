/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"

//==============================================================================
JDSRSynthAudioProcessor::JDSRSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", parameterLayout())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());// checked that all automaticly dealocate the memory
}

JDSRSynthAudioProcessor::~JDSRSynthAudioProcessor()
{
}

//==============================================================================
const juce::String JDSRSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JDSRSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JDSRSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JDSRSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JDSRSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JDSRSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JDSRSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JDSRSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JDSRSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void JDSRSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JDSRSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    keyboardState.reset();
    
    
    for (auto i = 0; i < synth.getNumVoices(); i++)
    {
        auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i));
        voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    }
}

void JDSRSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JDSRSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void JDSRSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i));
        
        auto gain = apvts.getRawParameterValue("GAIN");
        
        voice->updateGain(*gain);
        
        auto oscValue = apvts.getRawParameterValue("OSCILATOR");
        
        voice->updateOscType(*oscValue);
        
    }
    
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    
    synth.renderNextBlock(buffer, midiMessages,  0, buffer.getNumSamples());
}

//==============================================================================
bool JDSRSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JDSRSynthAudioProcessor::createEditor()
{
    return new JDSRSynthAudioProcessorEditor (*this);
}

//==============================================================================
void JDSRSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JDSRSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JDSRSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout JDSRSynthAudioProcessor::parameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.f, 1.f, 0.5f));
    
    parameters.push_back(std::make_unique<juce::AudioParameterChoice>("OSCILATOR", "Oscilator", juce::StringArray{"Sine" , "Piano", "Trumpet"}, 0));
    
    return {parameters.begin(), parameters.end()};
}

//audiosource
