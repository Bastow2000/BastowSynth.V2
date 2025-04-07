/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>
#include <vector>
#include "ParamId.h"
#include "WaveGen.h"
#include "Osc.h"

//==============================================================================
/**
*/
class BASAudioProcessor  : public juce::AudioProcessor, public juce::MidiKeyboardStateListener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    BASAudioProcessor();
    ~BASAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill);
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void initialiseSynth();
    void setWaveType(unsigned int index, int waveType);
   
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState tree;
    juce::MidiMessageCollector* getMidiCollector();
    juce::MidiKeyboardState keyboardState;
    juce::MidiMessageCollector midiCollector;
    std::unique_ptr<GenerateWavetable> generation;
private:

    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handlePitchWheel(juce::MidiKeyboardState*, int midiChannel, int wheelValue);
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message);
    void setMidiInput(int index);
    bool isAddingFromMidiInput;
    
    juce::Synthesiser synthesiser;
    juce::LinearSmoothedValue<float> smooth { 0.0f};
    int selectedOscillatorIndex = 1; // Calculating start index

    //ParameterNames parameterNames_;
    static const int kNumOscillators = 33;
    int numVoices;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BASAudioProcessor)
};
