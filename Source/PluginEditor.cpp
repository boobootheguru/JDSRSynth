/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JDSRSynthAudioProcessorEditor::JDSRSynthAudioProcessorEditor (JDSRSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 200, 100);
    addAndMakeVisible(gainSlider);
    
    oscilatorComboBox.addItem("Sine", 1);
    oscilatorComboBox.addItem("Square", 2);
    oscilatorComboBox.addItem("Saw", 3);
    addAndMakeVisible(&oscilatorComboBox);
    oscilatorComboBox.addListener(this);
    
    

    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    oscilatorComboBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSCILATOR", oscilatorComboBox);
    keyboardComponent = std::make_unique<juce::MidiKeyboardComponent>(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard);
    addAndMakeVisible(*keyboardComponent);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);
}

JDSRSynthAudioProcessorEditor::~JDSRSynthAudioProcessorEditor()
{
}

//==============================================================================
void JDSRSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void JDSRSynthAudioProcessorEditor::resized()
{
    const int margin = 20;
    const int sliderWidth = getWidth() / 2 - 2 * margin;
    const int sliderHeight = getHeight() - 2 * margin - 100;
    
    const int keyboardWidth = getWidth();
    const int keyboardHeight = 100;
    
    
    gainSlider.setBounds( margin, margin, sliderWidth, sliderHeight);
    oscilatorComboBox.setBounds(getWidth() / 2 + margin, margin, sliderWidth, sliderHeight );
    
    
    keyboardComponent->setBounds(0, getHeight() - keyboardHeight, keyboardWidth, keyboardHeight);
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void JDSRSynthAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    
}
