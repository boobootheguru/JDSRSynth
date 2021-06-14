/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class JDSRSynthAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                       private juce::ComboBox::Listener
{
public:
    JDSRSynthAudioProcessorEditor (JDSRSynthAudioProcessor&);
    ~JDSRSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox*) override;


private:
    juce::Slider gainSlider;
    juce::ComboBox oscilatorComboBox;
    //juce::MidiKeyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard);


    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscilatorComboBoxAttachment;
    std::unique_ptr<juce::MidiKeyboardComponent> keyboardComponent;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JDSRSynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JDSRSynthAudioProcessorEditor)
};
