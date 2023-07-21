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
    wavetable_ = wavetable;

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
    int indexBelow = readPointer_;
    int indexAbove = indexBelow + 1;
    indexAbove %= wavetable_.size();

    // Calculate the weights for the below and above samples
    float fractionAbove = readPointer_ - indexBelow;
    float fractionBelow = 1.0f - fractionAbove;

    // Load the wavetable values for the below and above indices using NEON intrinsics
    float32x2_t vBelow = vld1_f32 (&wavetable_[indexBelow]);
    float32x2_t vAbove = vld1_f32 (&wavetable_[indexAbove]);

    // Calculate the weighted average of the below and above samples using NEON intrinsics
    float32x2_t vFractionBelow = vdup_n_f32 (fractionBelow);
    float32x2_t vFractionAbove = vdup_n_f32 (fractionAbove);
    float32x2_t vWeightedBelow = vmul_f32 (vBelow, vFractionBelow);
    float32x2_t vWeightedAbove = vmul_f32 (vAbove, vFractionAbove);
    float32x2_t vResult = vadd_f32 (vWeightedBelow, vWeightedAbove);

    // Extract the result from the NEON register
    float result;
    vst1_f32 (&result, vResult);
    return result;
}

float Wavetable::process()
{
    float output = 0;

    readPointer_ += (phaseIncrement_ * frequency_);
    while (readPointer_ >= wavetable_.size())
        readPointer_ -= wavetable_.size();

    output = linearInterpolation();

    return output * amplitude_;
}
