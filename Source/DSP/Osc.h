#pragma once
#include "WaveGen.h"
#include "WaveSetup.h"
#include "ADSR.h"
#include "Ramp.h"
#include <JuceHeader.h>

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
        : wavetableSize_ (1024),
          waveType_ (kNumOscillators_, 4),
          gains_ (kNumOscillators_),
          sineTable_ (wavetableSize_),
          frequencyP_ (kNumOscillators_),
          frequencyN_ (kNumOscillators_),
          oscillators_ (kNumOscillators_)
    {
        // Initialize any necessary member variables
        CreateWaveTable();
        CreateOscillator();
        amplitudeADSR.setSampleRate((float) getSampleRate());
    }

    void promptInitialWavetable()
    {
        for (unsigned int n = 0; n < kNumOscillators_; ++n)
        {
            gains_[n] = 1.0f / kNumOscillators_; // Initial gain value
        }
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

    void setFrequency (unsigned int index, int frequency)
    {
        if (index < kNumOscillators_)
        {
            frequencyP_[index] = static_cast<int> (frequency) / 24;
        }
    }

    bool canPlaySound (juce::SynthesiserSound*) override { return true; }

    static float noteHz (int midiNoteNumber, double centsOffset)
    {
        double hertz = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        hertz *= std::pow (4.0, centsOffset / 4800);
        return (float) hertz;
    }

    void setWaveType (unsigned int index, int waveType)
    {
        // this is very confusing, is waveType changing size?
        if (4 < waveType_.size())
        {
            waveType_[index] = waveType;
            buttonPressed = true;
            std::cout << "The value of number is: " << waveType << std::endl;
            std::cout << "The vector of number is: " << waveType_[index] << std::endl;
        }
    }

    float pitchBendCents (int index)
    {
        if (frequencyP_[static_cast<std::vector<float>::size_type> (index)] >= 0.0f)
        {
            // shifting up
            return frequencyP_[static_cast<std::vector<float>::size_type> (index)] * pitchBendUpSemitones_ * 100;
        }
        else
        {
            // shifting down
            return -1 * frequencyP_[static_cast<std::vector<float>::size_type> (index)] * pitchBendDownSemitones_ * 100;
        }
    }

    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        amplitudeADSR.trigger();
        float oscillatorFrequency;
        float oscGain;

        for (unsigned int n = 0; n < kNumOscillators_; ++n)
        {
            float frequencyOffset = ((n + 1) * frequencyP_[n]) * (n * frequencyIncrement_);
            float harmonicFrequency = noteHz (midiNoteNumber, 0) * (n + 1); // calculate frequency for the current harmonic
            frequencyN_[n] = harmonicFrequency + frequencyOffset;
            level_ = velocity;
            for (unsigned int s = 0; s < 4; ++s)
            {
                oscillatorFrequency = frequencyN_[n];
                oscillators_[n]->setFrequency (oscillatorFrequency);

                oscGain = gains_[n];

                oscillators_[n]->setAmplitude (oscGain);
            }
        }
    }

    void stopNote (float /*velocity*/, bool /*allowTailOff*/) override
    {
        amplitudeADSR.release();
        clearCurrentNote();
    }

    void pitchWheelMoved (int /*newValue*/) override {}

    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override {}

    void setCustomADSRParameters(float att,float dec,float sus,float rel,float hold, float dec2)
    {
        amplitudeADSR.setAttackTime(att);
        amplitudeADSR.setAttackHoldTime(hold);
		amplitudeADSR.setDecayTime(dec);
        amplitudeADSR.setSustainLevel(sus);
		amplitudeADSR.setDecay2Time(dec2);
        amplitudeADSR.setReleaseTime(rel);
        amplitudeADSR.setSegment(dec2,rel, Release);
    }

    
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        std::array<float,kNumOscillators_> envelopeValues;
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float value = 0.0f;
            for (unsigned int i = 0; i < oscillators_.size(); ++i)
            {
                envelopeValues[i] = amplitudeADSR.process();
                value += oscillators_[i]->process() * (gains_[i] )  * (envelopeValues[i] );
            }

            value *= masterGain_;

                for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
                {
            
                    outputBuffer.addSample(channel, startSample + sample, value);
                }
            
        }
    }

    std::vector<int> waveType_;
    bool buttonPressed = false;

private:
    void CreateWaveTable()
    {
        sineTable_.clear();
        sineTable_.resize (wavetableSize_);
        generation_ = std::make_unique<GenerateWavetable> ((float) getSampleRate(), sineTable_, phase_);
        for (unsigned int n = 0; n < kNumOscillators_; ++n)
        {
            sineTable_ = generation_->prompt_Harmonics ((3));
        }
    }

    void CreateNewWaveTable()
    {
        sineTable_.clear();
        sineTable_.resize (wavetableSize_);
        generation_ = std::make_unique<GenerateWavetable> ((float) getSampleRate(), sineTable_, phase_);
        for (unsigned int n = 0; n < kNumOscillators_; ++n)
        {
            sineTable_ = generation_->prompt_Harmonics (static_cast<unsigned int> (waveType_[n]));
        }
    }
    void CreateOscillator()
    {
        CreateWaveTable();
        oscillators_.clear();
        oscillators_.resize (kNumOscillators_);
        for (unsigned int n = 0; n < kNumOscillators_; ++n)
        {
            oscillators_[n] = std::make_unique<Wavetable> ((float) getSampleRate(), sineTable_, phase_);
        }
    }

    void changeOscillator()
    {
        
            CreateNewWaveTable();
      
            oscillators_.clear();
            oscillators_.resize (kNumOscillators_);
            for (unsigned int n = 0; n < kNumOscillators_; ++n)
            {
                oscillators_[n] = std::make_unique<Wavetable> ((float) getSampleRate(), sineTable_, phase_);
        }
       
    }
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
    

    std::vector<std::unique_ptr<Wavetable>> oscillators_;
    std::unique_ptr<GenerateWavetable> generation_;
};