#pragma once
#include "WaveGen.h"
#include "WaveSetup.h"
#include "ADSR.h"
#include "Ramp.h"

#include <JuceHeader.h>
struct WaveTypes {
    enum Type {
        Sine = 1,
        Triangle = 2,
        Square = 3,
        Saw = 4
    };
};
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
        :  waveType_ (kNumOscillators_, 0),
        wavetableSize_ (1024),
        gains_ (kNumOscillators_),
        sineTable_ (wavetableSize_),
        squareTable_(wavetableSize_),
        triangleTable_ (wavetableSize_),
        sawTable_ (wavetableSize_),
        frequencyP_ (kNumOscillators_),
        frequencyN_ (kNumOscillators_),
        oscillators_ (kNumOscillators_)
    {
        // Initialize any necessary member variables
        CreateWaveTable();
        CreateOscillator();

        amplitudeADSR.setSampleRate((float) getSampleRate());
        
        if (message_.isNoteOn())
        {
            midiNoteNumber_ = message_.getNoteNumber();
            // Pass the MIDI note number to the synthesiser so it can play the note.
        }
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

    int getWaveType (int oscillatorIndex)
    {
        if (oscillatorIndex < kNumOscillators_)
            return waveType_[oscillatorIndex];
        return -1;
    }

    void addObserver(std::function<void(unsigned int, int)> observer) 
    {
        observers_.push_back(observer);
    }
    void setWaveType (unsigned int oscillatorIndex, int waveType)
    {
        if (oscillatorIndex < kNumOscillators_)
        {
            waveType_[oscillatorIndex] = waveType;
            
            printf("Oscillator %d, Wave Type %d\n", oscillatorIndex, waveType);
            // Assuming changeOscillator does the immediate change necessary
            changeOscillator(waveType, oscillatorIndex);
            notifyObservers(oscillatorIndex, waveType);
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
      
        oscillatorIndex_ =  (oscillatorIndex_+1) %4  ; 
        if (oscillatorIndex_ == 0) 
        {
            wavetype_ = (wavetype_ + 1) % 4; // Cycle through 4 wave types
        }
       
        changeOscillator(wavetype_, oscillatorIndex_);
        std::cout << "StartNote wavetype_: " << wavetype_ << ", oscillatorIndex_: " << oscillatorIndex_ << std::endl;

        //changeOscillator(wavetype_, oscillatorIndex_);
        for (unsigned int n = 0; n < kNumOscillators_; ++n)
        {
            
            
            float pitchHz = 440.0f * std::pow(2.0f, (midiNoteNumber - 69) / 12.0f) * (n+1);
            float harmonicFrequency =  pitchHz ; // calculate frequency for the current harmonic
            frequencyN_[n] = harmonicFrequency; //+ frequencyOffset;
            level_ = velocity;
           
                oscillatorFrequency = frequencyN_[n];
                oscillators_[n]->setFrequency (oscillatorFrequency);

                oscGain = gains_[n];

                oscillators_[n]->setAmplitude (oscGain);
               
            if ( n == oscillatorIndex_) 
            { // shouldUpdateAllOscillators is a conditional flag you define
              std::cout << "Updating oscillator " << n << " with new settings." << std::endl;
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
    int midiNoteNumber_;    
private:
    void notifyObservers(unsigned int oscillatorIndex, int waveType) 
    {
        for (auto& observer : observers_) 
        {
            observer(oscillatorIndex, waveType); // Notify each observer with the new values
        }
    }
    void CreateWaveTable()
    {
        sineTable_.clear();
        sineTable_.resize (wavetableSize_);
        generationSine_ = std::make_unique<GenerateWavetable> ((float) getSampleRate(), sineTable_, phase_,midiNoteNumber_,WaveTypes::Sine);
        generationSquare_ = std::make_unique<GenerateWavetable> ((float) getSampleRate(), squareTable_, phase_,midiNoteNumber_,WaveTypes::Square);
        generationTriangle_ = std::make_unique<GenerateWavetable> ((float) getSampleRate(), triangleTable_, phase_,midiNoteNumber_,WaveTypes::Triangle);
        generationSaw_ = std::make_unique<GenerateWavetable> ((float) getSampleRate(), sawTable_, phase_,midiNoteNumber_,WaveTypes::Saw);
        
        sineTable_ = generationSine_->prompt_Carrier ();
        squareTable_ = generationSquare_->prompt_Carrier ();
        triangleTable_ = generationTriangle_->prompt_Carrier ();
        sawTable_ = generationSaw_->prompt_Carrier ();
        
    }



    void CreateOscillator()
    {
        CreateWaveTable();
        oscillators_.clear();
        oscillators_.resize (kNumOscillators_);
        for (unsigned int n = 0; n < kNumOscillators_; ++n)
        {
            oscillators_[n] = std::make_unique<Wavetable> ((float) getSampleRate(), squareTable_, phase_);
        }
    }

    void changeOscillator(int waveNumber, int oscillatorIndex)
    {
        //CreateWaveTable();
       
        if (oscillatorIndex >= 0 && oscillatorIndex < kNumOscillators_)
        {
           // oscillators_.clear();
           // oscillators_.resize (kNumOscillators_);
            switch (waveNumber)
            {
                case WaveTypes::Sine: // Sine Wave
                   
                    oscillators_[oscillatorIndex] = std::make_unique<Wavetable> ((float) getSampleRate(), sineTable_, phase_);
                    break;
                case WaveTypes::Triangle: // Triangle Wave
                    
                    oscillators_[oscillatorIndex] = std::make_unique<Wavetable> ((float) getSampleRate(), squareTable_, phase_);
                    break;
                case WaveTypes::Square: // Square Wave
                   
                    oscillators_[oscillatorIndex] = std::make_unique<Wavetable> ((float) getSampleRate(), triangleTable_, phase_);
                    break;
                case WaveTypes::Saw: // Sawtooth Wave
                   
                    oscillators_[oscillatorIndex] = std::make_unique<Wavetable> ((float) getSampleRate(), sawTable_, phase_);
                    break;
            }
             std::cout << "Changed oscillator " << oscillatorIndex << " to wave type " << waveNumber << std::endl;
        }
    }
    void setShouldUpdateAllOscillators(bool shouldUpdate) 
    {
        shouldUpdateAllOscillators = shouldUpdate;
    }   
    static constexpr unsigned int kNumOscillators_ = 33; //33
    size_t wavetableSize_;
    float level_ = 0.0f;
    float phase_ = 0.0f;
    float masterGain_ = 0.0f;
    float frequencyIncrement_ = 5.2f;
    int pitchBendUpSemitones_ = 24;
    int pitchBendDownSemitones_ = 12;
    int oscillatorIndex_;
    int wavetype_;
    juce::ADSR adsr;
    CustomADSR amplitudeADSR;
    
    juce::ADSR::Parameters adsrParameters;
    std::vector<juce::ADSR::Parameters> adsrParams_;



    std::vector<float> gains_;
    std::vector<float> sineTable_;
    std::vector<float> triangleTable_;
    std::vector<float> sawTable_;
    std::vector<float> squareTable_;

    std::vector<float> frequencyP_;
    std::vector<float> frequencyN_;
    

    std::vector<std::unique_ptr<Wavetable>> oscillators_;
    juce::MidiMessage message_; 

      std::vector<std::function<void(unsigned int, int)>> observers_; // Observers interested in wave type changes

    bool shouldUpdateAllOscillators = false;
    std::unique_ptr<GenerateWavetable> generationSine_;
    std::unique_ptr<GenerateWavetable> generationSquare_;
    std::unique_ptr<GenerateWavetable> generationSaw_;
     std::unique_ptr<GenerateWavetable> generationTriangle_;
};