/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "BASPluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BASAudioProcessor::BASAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
          ),
      tree(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    keyboardState.addListener(this);

    numVoices = 4;
    initialiseSynth();
}

BASAudioProcessor::~BASAudioProcessor() {}

void BASAudioProcessor::initialiseSynth()
{
    synthesiser.clearVoices();
    synthesiser.clearSounds();
    for (int i = numVoices; --i >= 0;)
    {
        synthesiser.addVoice(new WavetableSynthesiserVoice());
    }

    synthesiser.addSound(new WavetableSynthesiserSound());
}

juce::AudioProcessorValueTreeState::ParameterLayout BASAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.reserve((3 * 33) + 10); // Reserve space for 33 sets of parameters

    // Helper lambdas for creating parameter IDs and names
    constexpr static auto createParamIdWithIndex = [](auto name, auto index) {
        return juce::ParameterID{std::string{name} + std::to_string(index), versionHint};
    };

    constexpr static auto createParameterNameWithIndex = [](auto name, auto index) {
        return concatenateStringAndInt(name, index);
    };

    for (int i = 0; i < 33; i++)
    {
        const auto gainId = createParamIdWithIndex(baseGainId, i);
        const auto gainName = createParameterNameWithIndex(baseGainName, i);

        const auto freqId = createParamIdWithIndex(baseFreqId, i);
        const auto freqName = createParameterNameWithIndex(baseFreqName, i);

        const auto wavetypeId = createParamIdWithIndex(baseWaveTypeId, i);
        const auto wavetypeName = createParameterNameWithIndex(baseWaveTypeName, i);

        auto gainParameter = std::make_unique<juce::AudioParameterFloat>(
            gainId, gainName, juce::NormalisableRange<float>(-64.0f, -12.0f, 1.0), -64.0f);

        auto freqParameter = std::make_unique<juce::AudioParameterFloat>(
            freqId, freqName, juce::NormalisableRange<float>(-24.0f, +24.0f, 0.0), 1.0f);

        auto waveTypeParameter = std::make_unique<juce::AudioParameterChoice>(
            wavetypeId,                // Parameter ID
            wavetypeName,              // Parameter name
            juce::StringArray{"Sine", "Triangle", "Square", "Saw"}, // Options
            0                          // Default option index
        );

        params.push_back(std::move(waveTypeParameter));
        params.push_back(std::move(gainParameter));
        params.push_back(std::move(freqParameter));
    }

    // Add additional parameters
    const auto masterGainParamId = juce::ParameterID{"gainIdM", versionHint};
    auto MasterGainParameter = std::make_unique<juce::AudioParameterFloat>(
        masterGainParamId, GainNameM, juce::NormalisableRange<float>(-60.0f, 6.0f, 1.0), -64.0f);

    // Component Volume Button1 - 3
    const auto cvb1ParamId = juce::ParameterID{cvb1Id, versionHint};
    auto CVB1 = std::make_unique<juce::AudioParameterBool>(cvb1ParamId, CVB1Name, false);
    const auto cvb2ParamId = juce::ParameterID{cvb2Id, versionHint};
    auto CVB2 = std::make_unique<juce::AudioParameterBool>(cvb2ParamId, CVB2Name, false);
    const auto cvb3ParamId = juce::ParameterID{cvb3Id, versionHint};
    auto CVB3 = std::make_unique<juce::AudioParameterBool>(cvb3ParamId, CVB3Name, false);

    auto Attack = std::make_unique<juce::AudioParameterFloat>(attackId, AttackName, juce::NormalisableRange<float>(0.001f, 20.f), 0.01f);
    auto Decay = std::make_unique<juce::AudioParameterFloat>(decayId, DecayName, juce::NormalisableRange<float>(0.01f, 15.f), 0.05f);
    auto Decay2 = std::make_unique<juce::AudioParameterFloat>(decay2Id, Decay2Name, juce::NormalisableRange<float>(0.01f, 20.f), 0.1f);
    auto Hold = std::make_unique<juce::AudioParameterFloat>(holdId, HoldName, juce::NormalisableRange<float>(0.0f, 15.f), 0.5f);
    auto Sustain = std::make_unique<juce::AudioParameterFloat>(sustainId, SustainName, juce::NormalisableRange<float>(0.0f, 15.f), 0.5f);
    auto Release = std::make_unique<juce::AudioParameterFloat>(releaseId, ReleaseName, juce::NormalisableRange<float>(0.01f, 25.f), 0.5f);

    params.push_back(std::move(CVB1));
    params.push_back(std::move(CVB2));
    params.push_back(std::move(CVB3));
    params.push_back(std::move(Attack));
    params.push_back(std::move(Decay));
    params.push_back(std::move(Decay2));
    params.push_back(std::move(Hold));
    params.push_back(std::move(Sustain));
    params.push_back(std::move(Release));
    params.push_back(std::move(MasterGainParameter));

    return {params.begin(), params.end()};
}


void BASAudioProcessor::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent(message);
}

void BASAudioProcessor::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    juce::MidiMessage m(juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity));
    synthesiser.noteOn(midiChannel, midiNoteNumber, velocity);
}

void BASAudioProcessor::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    juce::MidiMessage m(juce::MidiMessage::noteOff(midiChannel, midiNoteNumber, velocity));
    synthesiser.noteOff(midiChannel, midiNoteNumber, velocity, true);
}

void BASAudioProcessor::handlePitchWheel(juce::MidiKeyboardState*, int midiChannel, int wheelValue)
{
    synthesiser.handlePitchWheel(midiChannel, wheelValue);
}

//==============================================================================
const juce::String BASAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BASAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool BASAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool BASAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double BASAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BASAudioProcessor::getNumPrograms()
{
    return 1; 
}

int BASAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BASAudioProcessor::setCurrentProgram(int index) {}

const juce::String BASAudioProcessor::getProgramName(int index)
{
    return {};
}

void BASAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

//==============================================================================
void BASAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
    auto lastSampleRate = sampleRate;
    double currentGain = juce::Decibels::decibelsToGain(*tree.getRawParameterValue(gainIdM) + 0.0);
    smooth.reset(sampleRate, 0.1);
    smooth.setTargetValue(static_cast<float>(currentGain));
    smooth.getNextValue();
    synthesiser.setCurrentPlaybackSampleRate(lastSampleRate);

    // Ensure voices are added to the synthesizer
    for (int i = 0; i < 8; ++i) // Adjust the number of voices as needed
    {
        synthesiser.addVoice(new WavetableSynthesiserVoice());
    }

    synthesiser.addSound(new WavetableSynthesiserSound());
}


void BASAudioProcessor::releaseResources()
{
    synthesiser.clearVoices();
    synthesiser.clearSounds();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BASAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

  
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void BASAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    for (int i = 0; i < synthesiser.getNumVoices(); i++)
    {
        if (auto* voice = dynamic_cast<WavetableSynthesiserVoice*>(synthesiser.getVoice(i)))
        {
            double currentGainMaster = juce::Decibels::decibelsToGain(*tree.getRawParameterValue(gainIdM) + 0.0);
            voice->setMasterGain(static_cast<float>(currentGainMaster));

            float attack = *tree.getRawParameterValue(attackId);
            float release = *tree.getRawParameterValue(releaseId);
            float sustain = *tree.getRawParameterValue(sustainId);
            float decay = *tree.getRawParameterValue(decayId);
            float hold = *tree.getRawParameterValue(holdId);
            float decay2 = *tree.getRawParameterValue(decay2Id);

            voice->setCustomADSRParameters(attack, hold, decay, sustain, decay2, release);

            std::array<float, kNumOscillators> currentGain;
            std::array<int, kNumOscillators> frequency;
            std::array<int, kNumOscillators> waveType;

            for (unsigned int n = 0; n < kNumOscillators; ++n)
            {
                currentGain[n] = juce::Decibels::decibelsToGain(*tree.getRawParameterValue("gainId" + std::to_string(n)) + 0.0);
                frequency[n] = static_cast<int>(*tree.getRawParameterValue("freqId" + std::to_string(n)));
                waveType[n] = static_cast<int>(*tree.getRawParameterValue("waveTypeId" + std::to_string(n)));

                voice->setWaveType(n, waveType[n]);
                voice->setGain(n, currentGain[n]);
                voice->setFrequency(n, frequency[n]);
            }
        }
    }

    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void BASAudioProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    juce::MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
    keyboardState.processNextMidiBuffer(incomingMidi, 0, bufferToFill.numSamples, true);

    synthesiser.renderNextBlock(*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
}

juce::MidiMessageCollector* BASAudioProcessor::getMidiCollector()
{
    return &midiCollector;
}

//==============================================================================
bool BASAudioProcessor::hasEditor() const
{
    return true; 
}

juce::AudioProcessorEditor* BASAudioProcessor::createEditor()
{
    return new BASAudioProcessorEditor(*this);
}

//==============================================================================
void BASAudioProcessor::getStateInformation(juce::MemoryBlock& /*destData*/)
{
    

void BASAudioProcessor::setStateInformation(const void* /*data*/, int /*sizeInBytes*/)
{
   
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BASAudioProcessor();
}
