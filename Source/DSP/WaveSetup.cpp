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

void Wavetable::setNewWavetable(std::vector<float> newWavetable) { // ✅ Copy/move
    wavetable_ = std::move(newWavetable); // Efficient transfer
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

void Wavetable::setSampleRate (const float newSampleRate) {
    sampleRate_ = newSampleRate;
    inverseSampleRate_ = 1.0f / newSampleRate;
    phaseIncrement_ = (wavetable_.size() * inverseSampleRate_);
}

void Wavetable::updatePhaseIncrement() {
    phaseIncrement_ = (wavetable_.size() * inverseSampleRate_);
}

// Get the oscillator frequency
float Wavetable::getFrequency() const
{
    return frequency_;
}

// Get the oscillator amplitude
float Wavetable::getAmplitude() const
{
    return amplitude_;
}

void Wavetable::setCustomADSRParameters(float att, float dec, float sus, float rel, float hold, float dec2)
{
    amplitudeADSR.setAttackTime(att);
    amplitudeADSR.setDecayTime(dec);
    amplitudeADSR.setSustainLevel(sus);
    amplitudeADSR.setReleaseTime(rel);
    amplitudeADSR.setAttackHoldTime(hold);
    amplitudeADSR.setDecay2Time(dec2);
    amplitudeADSR.setSegment(dec2,rel, Release);
}


// Get the next sample and update the phase

float Wavetable::linearInterpolation() const
{
    if (wavetable_.empty()) return 0.0f;

    const int indexBelow = static_cast<int>(readPointer_);
    const int indexAbove = (indexBelow + 1) % wavetable_.size();
    const float fraction = readPointer_ - indexBelow;

    return wavetable_[indexBelow] * (1.0f - fraction) +
           wavetable_[indexAbove] * fraction;
}

float Wavetable::cubicInterpolation()
{
    const int index = static_cast<int>(readPointer_);
    const float frac = readPointer_ - index;

    const int index0 = (index - 1 + wavetable_.size()) % wavetable_.size();
    const int index1 = index % wavetable_.size();
    const int index2 = (index + 1) % wavetable_.size();
    const int index3 = (index + 2) % wavetable_.size();

    const float y0 = wavetable_[index0];
    const float y1 = wavetable_[index1];
    const float y2 = wavetable_[index2];
    const float y3 = wavetable_[index3];

    // Cubic interpolation formula (e.g., Catmull-Rom)
    const float c0 = y1;
    const float c1 = 0.5f * (y2 - y0);
    const float c2 = y0 - 2.5f * y1 + 2.0f * y2 - 0.5f * y3;
    const float c3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);

    return ((c3 * frac + c2) * frac + c1) * frac + c0;
}



float Wavetable::process()
{
    float output = 0;

    readPointer_ += (phaseIncrement_ * frequency_);
    while (readPointer_ >= wavetable_.size())
        readPointer_ -= wavetable_.size();

    output = cubicInterpolation();

    return output * amplitude_;
}
