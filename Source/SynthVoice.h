/*
  ==============================================================================

    SynthVoice.h
    Created: 6 Jun 2021 1:00:05pm
    Author:  Rayan El Koudsi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound * synthsound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *synthsound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void updateGain (const float gain);
    void updateOscType (float selection);
    
    juce::dsp::Oscillator<float> osc;
    
private:
    int wave;
    
    juce::ADSR adsr;
    juce::ADSR::Parameters  adsrParameters;
    
    juce::dsp::Gain<float> gain;
    
    juce::AudioBuffer<float> synthBuffer;
    
    juce::MidiKeyboardState midiKeyboardState;
};
