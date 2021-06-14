/*
  ==============================================================================

    SynthSound.h
    Created: 6 Jun 2021 1:00:22pm
    Author:  Rayan El Koudsi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
    bool appliesToNote (int midiNoteNumber) override {return true;}
    bool appliesToChannel (int midiChannel) override {return true;}
};
