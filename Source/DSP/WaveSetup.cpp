#include "WaveSetup.h"

// Constructor taking arguments for sample rate and table data
// Modified class from week 3 lecture
Wavetable::Wavetable (float sampleRate, std::vector<float>& wavetable, float phase)
{
    setup (sampleRate, wavetable, phase);
}

void Wavetable::setup (float sampleRate, std::vector<float>& wavetable, float phase)
{
    // It's faster to multiply than to divide on most platforms, so we save the inverse
    // of the sample rate for use in the phase calculation later
    inverseSampleRate_ = 1.0 / sampleRate;

    // Copy other parameters
    this->wavetable_ = wavetable;

    phase_ = phase;

    //calculates the phase increment for a waveform table of a given size and sample rate,
    phaseIncrement_ = (wavetable.size() * inverseSampleRate_);

    // Initialise the starting state
    readPointer_ = 0;
}

void Wavetable::setNewWavetable (const std::vector<float>& newWavetable)
{
    wavetable_ = newWavetable;
    readPointer_ = 0;
}

// Set the oscillator frequency
void Wavetable::setFrequency (float f)
{
    frequency_ = f;
}

// Set the oscillator amplitude
void Wavetable::setAmplitude (float a)
{
    amplitude_ = a;
}

// Get the oscillator frequency
float Wavetable::getFrequency()
{
    return frequency_;
}

// Get the oscillator amplitude
float Wavetable::getAmplitude()
{
    return amplitude_;
}

void Wavetable::setCustomADSRParameters(float att, float dec, float sus, float rel, float hold, float dec2)
{
     amplitudeADSR.setAttackTime(att);
        amplitudeADSR.setAttackHoldTime(hold);
		amplitudeADSR.setDecayTime(dec);
        amplitudeADSR.setSustainLevel(sus);
		amplitudeADSR.setDecay2Time(dec2);
        amplitudeADSR.setReleaseTime(rel);
        amplitudeADSR.setSegment(dec2,rel, Release);
}


// Get the next sample and update the phase
float Wavetable::linearInterpolation()
{
	// Calculate the index below and above the read pointer
    int indexBelow = readPointer_;
    
    int indexAbove = indexBelow + 1;
    
    indexAbove %= wavetable_.size();
    
    // Calculate the weights for the below and above samples
    float fractionAbove = readPointer_ - indexBelow;
    
    float fractionBelow = 1.0 - fractionAbove;
    
    // Calculate the weighted average of the below and above samples
    return fractionBelow * wavetable_[indexBelow] + fractionAbove * wavetable_[indexAbove];
}		


float Wavetable::process()
{
    float output = 0;
    
    // increments the readpointer based on phase 
    readPointer_ += (phaseIncrement_ * frequency_);
    
    // When the readpointer exceeds the wavetable size 
    //reset the read pointer
    while (readPointer_ >= wavetable_.size())
        readPointer_ -= wavetable_.size();

    output = linearInterpolation();

    return output * amplitude_;
}
