#pragma once
#include "ADSR.h"
#include "Ramp.h"
#include "WaveGen.h"
#include "WaveSetup.h"
#include <JuceHeader.h>
#include <sys/fcntl.h>

class WavetableSynthesiserSound : public juce::SynthesiserSound
{
public:
bool appliesToNote (int /*midiNoteNumber*/) override { return true; }
bool appliesToChannel (int /*midiChannel*/) override { return true; }
};

class WavetableSynthesiserVoice : public juce::SynthesiserVoice
{
public:
WavetableSynthesiserVoice()
: wavetableSize_ (2048),
waveType_ (kNumOscillators_, 4),
gains_ (kNumOscillators_),
sineTable_ (wavetableSize_),
frequencyP_ (kNumOscillators_),
frequencyN_ (kNumOscillators_),
oscillators_ (kNumOscillators_)
{
    CreateNewWaveTable();
    changeOscillator();
    amplitudeADSR.setSampleRate((float) getSampleRate());
}

~WavetableSynthesiserVoice() override
{
    // Clean up resources
    oscillators_.clear();
    generation_.reset();
}

void promptInitialWavetable()
{

    float totalGain = 0.0f;
    for (unsigned int n = 0; n < kNumOscillators_; ++n)
    {
        gains_[n] = 1.0f / kNumOscillators_;
        totalGain += gains_[n];
    }
    // Optional: Normalize if needed
    if (totalGain > 1.0f)
        for (auto& gain : gains_)
            gain /= totalGain;
}

void setMasterGain (float masterVol)
{
    masterGain_ = masterVol * level_;
}

void setGain (unsigned int index, float gain)
{
    if (index < kNumOscillators_)
        gains_[index] = gain;
}

void setFrequency (unsigned int index, float frequency)
{
    if (index < kNumOscillators_)
    {
        frequencyP_[index] = (frequency);


        //std::cout<<"freqP:"<< frequencyP_[index]<<std::endl;
      //  std::cout<<"freqPs: "<< frequency<<std::endl;
    }
}


void CreateWaveTable()
{
    sineTable_.clear();
    sineTable_.resize (wavetableSize_);
    generation_ = std::make_unique<GenerateWavetable> (static_cast<float> (getSampleRate()), sineTable_, phase_);
    for (unsigned int n = 0; n < kNumOscillators_; ++n)
    {
        std::cout<<"freq"<< frequencyP_[n]<<std::endl;

        sineTable_ = generation_->prompt_Harmonics ((frequencyP_[n]));
    }
}

bool canPlaySound (juce::SynthesiserSound*) override { return true; }

static float noteHz (int midiNoteNumber, double centsOffset)
{
    double hertz = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    hertz *= std::pow (4.0, centsOffset / 4800);
    return (float) hertz;
}

void setWaveType(unsigned int index, int waveType)
{
    /*
    if (index < waveType_.size())
    {
        waveType_[index] = waveType;
        buttonPressed = true;
        CreateNewWaveTable(); // Regenerate wavetables
    }*/
}

float pitchBendCents (int index)
{
    /*
    if (frequencyP_[static_cast<std::vector<float>::size_type> (index)] >= 0.0f)
    {
        // shifting up
        return frequencyP_[static_cast<std::vector<float>::size_type> (index)] * pitchBendUpSemitones_ * 100;
    }
    else
    {
        // shifting down
        return -1 * frequencyP_[static_cast<std::vector<float>::size_type> (index)] * pitchBendDownSemitones_ * 100;
    }*/
}

void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
{
    amplitudeADSR.trigger();
    level_ = velocity;
    const float fundamental = noteHz(midiNoteNumber, 0);
    const float nyquist = getSampleRate() / 2.0f;
    float oscillatorFrequency;
    float oscGain;

    for (unsigned int n = 0; n < kNumOscillators_; ++n)
    {
        float frequencyOffset = ((n + 1) * frequencyP_[n]) * (n * frequencyIncrement_);
         const float harmonicFrequency = fundamental * (n + 1); // calculate frequency for the current harmonic
        frequencyN_[n] = harmonicFrequency + frequencyOffset;
        level_ = velocity;
        if (harmonicFrequency >= nyquist)
        {
            gains_[n] = 0.0f;
            continue;
        }
        for (unsigned int s = 0; s < 4; ++s)
        {
            oscillatorFrequency = frequencyN_[n];
            oscillators_[n]->setFrequency (oscillatorFrequency);

            oscGain = gains_[n];

            oscillators_[n]->setAmplitude (oscGain);
        }
    }
}

void stopNote (float /*velocity*/, bool allowTailOff) override
{

    if (allowTailOff)
        amplitudeADSR.release();
    else
    {
        amplitudeADSR.reset();
        clearCurrentNote();
    }
    CreateNewWaveTable();
    changeOscillator();
}

void pitchWheelMoved (int /*newValue*/) override {}

void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override {}

void setCustomADSRParameters(float att,float dec,float sus,float rel,float hold, float dec2)
{
    amplitudeADSR.setAttackTime(att);
	amplitudeADSR.setDecayTime(hold);
    amplitudeADSR.setSustainLevel(dec);

    amplitudeADSR.setReleaseTime(sus);
    amplitudeADSR.setDecay2Time(dec2);
    amplitudeADSR.setDecay2Time(rel);
    amplitudeADSR.setSegment(dec2,rel, Release);
}


void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
{


    bool envelopeComplete = true;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float env = amplitudeADSR.process();
        envelopeComplete &= (env <= 0.0f);

        float value = 0.0f;
        for (unsigned int i = 0; i < oscillators_.size(); ++i)
        {
            if (gains_[i] > 0.0f)
                value += oscillators_[i]->process() * gains_[i] * env;
        }

        value *= masterGain_;

        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            outputBuffer.addSample(channel, startSample + sample, value);
    }

    if (envelopeComplete)
        clearCurrentNote();
}

std::vector<int> waveType_;
bool buttonPressed = false;
private:


void CreateOscillator()
{
    CreateWaveTable();
    oscillators_.clear();
    oscillators_.resize (kNumOscillators_);
    for (unsigned int n = 0; n < kNumOscillators_; ++n)
    {
        oscillators_[n] = std::make_unique<Wavetable> (static_cast<float> (getSampleRate()), sineTable_, phase_);
    }
}
void CreateNewWaveTable()
{
// Resize the wavetable array to match the number of oscillators
oscillatorWavetables_.resize(kNumOscillators_);


    for (unsigned int n = 0; n < kNumOscillators_; ++n)
    {
        // Generate a new wavetable for each oscillator
        std::vector<float> wavetable(wavetableSize_);
        auto generation = std::make_unique<GenerateWavetable>(getSampleRate(), wavetable, phase_);

        // Generate the wavetable based on the current frequency parameters
        oscillatorWavetables_[n] = generation->prompt_Harmonics(static_cast<unsigned int>(frequencyP_[n]));
    }
}

void changeOscillator()
{
    // Ensure the oscillators array matches the number of oscillators
    for (auto& oscillator : oscillators_)
    {
        oscillator.reset(); // Release the old Wavetable object
    }
    oscillators_.clear();
    oscillators_.resize(kNumOscillators_);

    for (unsigned int n = 0; n < kNumOscillators_; ++n)
    {
        // Initialize each oscillator with the newly generated wavetable
        oscillators_[n] = std::make_unique<Wavetable>(
            static_cast<float>(getSampleRate()),
            oscillatorWavetables_[n],
            phase_
        );
    }
}
std::vector<std::vector<float>> oscillatorWavetables_;
static constexpr unsigned int kNumOscillators_ = 33;
size_t wavetableSize_;
float level_ = 0.0f;
float phase_ = 0.0f;
float masterGain_ = 0.0f;
float frequencyIncrement_ = 5.2f;
int pitchBendUpSemitones_ = 24;
int pitchBendDownSemitones_ = 12;
juce::ADSR adsr;
CustomADSR amplitudeADSR;

juce::ADSR::Parameters adsrParameters;
std::vector<juce::ADSR::Parameters> adsrParams_;

std::vector<float> gains_;
std::vector<float> sineTable_;
std::vector<float> frequencyP_;
std::vector<float> frequencyN_;
bool wavetableGenerated_ = false;

std::vector<std::unique_ptr<Wavetable>> oscillators_;
std::unique_ptr<GenerateWavetable> generation_;
};