#include "WaveGen.h"
// Constructor taking arguments for sample rate, wavetable data and phase
GenerateWavetable::GenerateWavetable (const float sampleRate, std::vector<float> wavetable, float phase):
wavetable_(std::move(wavetable))
{
    setup (sampleRate, phase);
}

// Setting up wavetable data, phase and sampleRate to be used in functions & their calls
void GenerateWavetable::setup (float sampleRate, float phase)
{
    // It's faster to multiply than to divide on most platforms, so we save the inverse
    // of the sample rate for use in the phase calculation later
    inverseSampleRate_ = 1.0f / sampleRate;

    // Assign parameters
    sampleRate_ = sampleRate;

    phase_ = phase;

    // Calculates the phase increment for a waveform table of a given size and sample rate
    phaseIncrement_ = (wavetable_.size() * inverseSampleRate_);
}

// Calculates poly_blep (Martin Finke,(no date))
float GenerateWavetable::poly_blep(float t, float dt)
{
    // Calculate the poly_blep for a single sample
    if (t < dt) {
        t /= dt;
        return t + t - t * t - 1.0f;
    }
    else if (t > 1.0f - dt) {
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }
    return 0.0f;
}

float GenerateWavetable::prompt_WaveType(unsigned int waveNumber, float n) {
    float t = static_cast<float>(n) / static_cast<float>(wavetable_.size());
    float value = 0.0f;
    float dt = phaseIncrement_ / static_cast<float>(wavetable_.size());

    switch (waveNumber) {
        case 1: // Sine
            value = sinf(2.0f * M_PI * t);
        break;
        case 2: // Triangle
            value = 2.0f * (0.5f - fabs(2.0f * t - 1.0f)) - 0.5f;
        break;
        case 3: // Square (using polyBLEP)
            value = (t < 0.5f) ? 1.0f : -1.0f;
        value += poly_blep(t, dt);
        value -= poly_blep(fmod(t + 0.5f, 1.0f), dt);
        break;
        case 4: // Sawtooth (using polyBLEP)
            value = 2.0f * t - 1.0f;
        value -= poly_blep(t, dt);
        break;
    }
    return value;
}

// Uses the wavetable_ defined from the constuctor to assign a wave type to wavetable_ then adds the
// wavetable_ to <Wavetable*> and sends the <Wavetable*> back to render to be used
std::vector<std::unique_ptr<Wavetable>> GenerateWavetable::prompt_Modulator (std::vector<std::unique_ptr<Wavetable>> gOscillators, unsigned int waveNumber)
{
    for (unsigned int n = 0; n < wavetable_.size(); n++)
    {
        wavetable_[n] = prompt_WaveType (waveNumber, n);
    }
    auto oscillator = std::make_unique<Wavetable>(sampleRate_, wavetable_, phase_);
    gOscillators.push_back(std::move(oscillator));

    return gOscillators;
}

// Uses the wavetable_ defined from the constuctor to assign a wave type to wavetable_ then returns value
std::vector<float> GenerateWavetable::prompt_Harmonics (unsigned int waveNumber)
{
    for (unsigned int n = 0; n < wavetable_.size(); n++)
    {
        wavetable_[n] = prompt_WaveType (waveNumber, n);
    }
    return wavetable_;
}
