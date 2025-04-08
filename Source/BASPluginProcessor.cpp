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
    : AudioProcessor (
        BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
            .withInput ("Input", juce::AudioChannelSet::stereo(), true)
#endif
            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
            ),
      tree (*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    keyboardState.addListener (this);

    numVoices = 4;
    initialiseSynth();
}

BASAudioProcessor::~BASAudioProcessor()
{

}

void BASAudioProcessor::initialiseSynth()
{
    synthesiser.clearVoices();
    synthesiser.clearSounds();

    // Add voices
    for (int i = 0; i < numVoices; ++i) // Fixed loop direction
    {
        synthesiser.addVoice(new WavetableSynthesiserVoice());
    }
    synthesiser.addSound(new WavetableSynthesiserSound());
}

juce::AudioProcessorValueTreeState::ParameterLayout
BASAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.reserve((3 * 33) + 10);

    constexpr static auto createParamIdWithIndex = [](auto name, auto index) {
        return juce::ParameterID{std::string{name} + std::to_string(index), versionHint};
    };

    constexpr static auto createParameterNameWithIndex = [](auto name, auto index) {
        return concatenateStringAndInt(name, index);
    };

    // Oscillator parameters
    for (int i = 0; i < 33; i++) {
        const auto gainId = createParamIdWithIndex(baseGainId, i);
        const auto gainName = createParameterNameWithIndex(baseGainName, i);
        const auto freqId = createParamIdWithIndex(baseFreqId, i);
        const auto freqName = createParameterNameWithIndex(baseFreqName, i);
        const auto wavetypeId = createParamIdWithIndex(baseWaveTypeId, i);
        const auto wavetypeName = createParameterNameWithIndex(baseWaveTypeName, i);

        auto gainParameter = std::make_unique<juce::AudioParameterFloat>(
            gainId, gainName, juce::NormalisableRange<float>(-64.0f, -12.0f, 1.0), -64.0f);

        auto freqParameter = std::make_unique<juce::AudioParameterFloat>(
            freqId, freqName, juce::NormalisableRange<float>(0.0f, 3.0f, 0.1), 2.5f);

        auto waveTypeParameter = std::make_unique<juce::AudioParameterBool>(
            wavetypeId, wavetypeName, false);

        params.push_back(std::move(waveTypeParameter));
        params.push_back(std::move(gainParameter));
        params.push_back(std::move(freqParameter));
    }

    // Master gain
    const auto masterGainParamId = juce::ParameterID{"gainIdM", versionHint};
    auto MasterGainParameter = std::make_unique<juce::AudioParameterFloat>(
        masterGainParamId, GainNameM, juce::NormalisableRange<float>(-60.0f, 6.0f, 1.0), 0.0f);

    // CVB buttons
    const auto cvb1ParamId = juce::ParameterID{cvb1Id, versionHint};
    auto CVB1 = std::make_unique<juce::AudioParameterBool>(cvb1ParamId, CVB1Name, false);
    const auto cvb2ParamId = juce::ParameterID{cvb2Id, versionHint};
    auto CVB2 = std::make_unique<juce::AudioParameterBool>(cvb2ParamId, CVB2Name, false);
    const auto cvb3ParamId = juce::ParameterID{cvb3Id, versionHint};
    auto CVB3 = std::make_unique<juce::AudioParameterBool>(cvb3ParamId, CVB3Name, false);

    // ADSR parameters
    const auto attackParamId = juce::ParameterID{attackId, versionHint};
    auto Attack = std::make_unique<juce::AudioParameterFloat>(
        attackParamId, AttackName, juce::NormalisableRange<float>(0.001f, 20.f), 0.01f);

    const auto decayParamId = juce::ParameterID{decayId, versionHint};
    auto Decay = std::make_unique<juce::AudioParameterFloat>(
        decayParamId, DecayName, juce::NormalisableRange<float>(0.01f, 15.f), 0.05f);

    const auto decay2ParamId = juce::ParameterID{decay2Id, versionHint};
    auto Decay2 = std::make_unique<juce::AudioParameterFloat>(
        decay2ParamId, Decay2Name, juce::NormalisableRange<float>(0.01f, 20.f), 0.1f);

    const auto holdParamId = juce::ParameterID{holdId, versionHint};
    auto Hold = std::make_unique<juce::AudioParameterFloat>(
        holdParamId, HoldName, juce::NormalisableRange<float>(0.0f, 15.f), 0.5f);

    const auto sustainParamId = juce::ParameterID{sustainId, versionHint};
    auto Sustain = std::make_unique<juce::AudioParameterFloat>(
        sustainParamId, SustainName, juce::NormalisableRange<float>(0.0f, 15.f), 0.5f);

    const auto releaseParamId = juce::ParameterID{releaseId, versionHint};
    auto Release = std::make_unique<juce::AudioParameterFloat>(
        releaseParamId, ReleaseName, juce::NormalisableRange<float>(0.01f, 25.f), 0.5f);

    // Add all parameters to vector
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

void BASAudioProcessor::handleIncomingMidiMessage (
    juce::MidiInput* source,
    const juce::MidiMessage& message)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput,
                                                         true);
    keyboardState.processNextMidiEvent (message);
}

void BASAudioProcessor::handleNoteOn (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    const float fixedVelocity = 1.0f; // Force full velocity

    juce::MidiMessage m (
        juce::MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
    synthesiser.noteOn (midiChannel, midiNoteNumber, fixedVelocity);
}

void BASAudioProcessor::handleNoteOff (juce::MidiKeyboardState*,
                                       int midiChannel,
                                       int midiNoteNumber,
                                       float velocity)
{
    juce::MidiMessage m (
        juce::MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity));
    synthesiser.noteOff (midiChannel, midiNoteNumber, velocity, true);
}

void BASAudioProcessor::handlePitchWheel (juce::MidiKeyboardState*,
                                          int midiChannel,
                                          int wheelValue)
{
    synthesiser.handlePitchWheel (midiChannel, wheelValue);
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

double BASAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int BASAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0
        // programs, so this should be at least 1, even if you're not really
        // implementing programs.
}

int BASAudioProcessor::getCurrentProgram() { return 0; }

void BASAudioProcessor::setCurrentProgram (int index) {}

const juce::String BASAudioProcessor::getProgramName (int index) { return {}; }

void BASAudioProcessor::changeProgramName (int index,
                                           const juce::String& newName) {}

//==============================================================================
void BASAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    juce::ignoreUnused (samplesPerBlock);
    auto lastSampleRate = sampleRate;
    double currentGain =
        juce::Decibels::decibelsToGain (*tree.getRawParameterValue (gainIdM) + 0.0);
    smooth.reset (sampleRate, 0.1);
    smooth.setTargetValue (static_cast<float> (currentGain));
    smooth.getNextValue();
    synthesiser.setCurrentPlaybackSampleRate (lastSampleRate);
}

void BASAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BASAudioProcessor::isBusesLayoutSupported (
    const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void BASAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                      juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    for (int i = 0; i < synthesiser.getNumVoices(); i++)
    {
        // `voice` is now strictly local to this block
        if (auto* voice = dynamic_cast<WavetableSynthesiserVoice*>(synthesiser.getVoice(i)))
        {
            // Master gain
            double currentGainMaster = juce::Decibels::decibelsToGain(
                *tree.getRawParameterValue(gainIdM) + 0.0
            );
            voice->setMasterGain(static_cast<float>(currentGainMaster));

            // ADSR parameters
            float attack  = *tree.getRawParameterValue(attackId);
            float release = *tree.getRawParameterValue(releaseId);
            float sustain = *tree.getRawParameterValue(sustainId);
            float decay   = *tree.getRawParameterValue(decayId);
            float hold    = *tree.getRawParameterValue(holdId);
            float decay2  = *tree.getRawParameterValue(decay2Id);

            voice->setCustomADSRParameters(attack, hold, decay, sustain, decay2, release);

            // Oscillator parameters
            for (unsigned int n = 0; n < 33; ++n)
            {
                // Gain
                float currentGain = juce::Decibels::decibelsToGain(
                    *tree.getRawParameterValue("gainId" + std::to_string(n)) + 0.0
                );
                voice->setGain(n, currentGain);

                // Frequency
                float frequency = static_cast<float>(
                    *tree.getRawParameterValue("freqId" + std::to_string(n))
                );
                voice->setFrequency(n, frequency);

                // Wave type (button)
                bool buttonPressed = (
                    *tree.getRawParameterValue("waveTypeId" + std::to_string(n)) > 0.5f
                );
                voice->buttonPressed = buttonPressed;
            }
        }
    }

    // Render synth output
    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void BASAudioProcessor::getNextAudioBlock (
    const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    juce::MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages (incomingMidi,
                                             bufferToFill.numSamples);
    keyboardState.processNextMidiBuffer (incomingMidi, 0, bufferToFill.numSamples, true);

    // synthesiser.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0,
    // bufferToFill.numSamples);
}

juce::MidiMessageCollector* BASAudioProcessor::getMidiCollector()
{
    return &midiCollector;
}

//==============================================================================
bool BASAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BASAudioProcessor::createEditor()
{
    return new BASAudioProcessorEditor (*this);
}

//==============================================================================
void BASAudioProcessor::getStateInformation (juce::MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BASAudioProcessor::setStateInformation (const void* /*data*/,
                                             int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BASAudioProcessor();
}
