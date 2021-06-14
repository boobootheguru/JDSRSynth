/*
  ==============================================================================

    SynthVoice.cpp
    Created: 6 Jun 2021 1:00:05pm
    Author:  Rayan El Koudsi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"


bool SynthVoice::canPlaySound (juce::SynthesiserSound * synthsound)
{
    return dynamic_cast<juce::SynthesiserSound*> (synthsound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *synthsound, int currentPitchWheelPosition)
{
    adsr.noteOn();
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}
void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    if (! isVoiceActive()) return;
    
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    
    synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock {synthBuffer};
    
    if (wave == 0)
    {
        osc.initialise([](float x) {return std::sin (x);});
    }
    else if (wave == 1)
    {
        osc.initialise([](float x) {  return x < 1.0f ? -1.0f : 1.0f;});
    }
    else if (wave == 2)
    {
        osc.initialise([](float x) { return x / juce::MathConstants<float>::pi;});
    }

    osc.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    gain.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    adsr.applyEnvelopeToBuffer (synthBuffer, 0, synthBuffer.getNumSamples());
    
    outputBuffer.addFrom(0, startSample, synthBuffer, 0, 0, numSamples);
        
    if (!adsr.isActive()) clearCurrentNote();
    
}
void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate (sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
        
    osc.prepare(spec);
    gain.prepare(spec);
    
    adsrParameters.attack = 0.2f;
    adsrParameters.decay = 0.2f;
    adsrParameters.sustain = 1.0f;
    adsrParameters.release = 0.8f;
}
void SynthVoice::updateGain (const float gainParam)
{
    gain.setGainLinear(gainParam);
}

void SynthVoice::updateOscType(float selection)
{
    wave = selection;
}
