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

float GenerateWavetable::prompt_WaveType(float waveNumber, float n) {
    float dt = phaseIncrement_ / static_cast<float>(wavetable_.size());
    float tableSize = static_cast<float>(wavetable_.size());
    float normalizedPosition = n / tableSize; // Full cycle phase [0, 1)

    std::vector<float> waveValues(4, 0.0f);

    // Sine wave (waveValues[0])
    waveValues[0] = sinf(2.0f * M_PI * normalizedPosition);

    // Triangle wave (waveValues[1])
    waveValues[1] = 2.0f * (0.5f - fabs(2.0f * normalizedPosition - 1.0f)) - 0.5f;

    // Square wave with polyBLEP (waveValues[2])
    waveValues[2] = (normalizedPosition < 0.5f) ? 1.0f : -1.0f;
    waveValues[2] += poly_blep(normalizedPosition, dt);
    waveValues[2] -= poly_blep(fmod(normalizedPosition + 0.5f, 1.0f), dt);

    // Sawtooth wave with polyBLEP (waveValues[3])
    waveValues[3] = 2.0f * normalizedPosition - 1.0f;
    waveValues[3] -= poly_blep(normalizedPosition, dt);

    // Clamp waveNumber to [0.0, 3.0] to stay within the 4 waveforms
    waveNumber = std::clamp(waveNumber, 0.0f, 3.0f);
    int waveIndex = static_cast<int>(waveNumber);
    float blend = waveNumber - waveIndex;

    // Edge case for the last waveform (sawtooth)
    if (waveIndex >= 3) {
        return waveValues[3];
    }

    // Linear interpolation between adjacent waveforms
    return (1.0f - blend) * waveValues[waveIndex] + blend * waveValues[waveIndex + 1];
}

// Updated to accept float waveNumber for interpolation
std::vector<float> GenerateWavetable::prompt_Harmonics(float waveNumber) {
    for (unsigned int n = 0; n < wavetable_.size(); n++) {
        wavetable_[n] = prompt_WaveType(waveNumber, static_cast<float>(n));
    }
    return wavetable_;
}
// Uses the wavetable_ defined from the constuctor to assign a wave type to wavetable_ then adds the
// wavetable_ to <Wavetable*> and sends the <Wavetable*> back to render to be used
std::vector<std::unique_ptr<Wavetable>> GenerateWavetable::prompt_Modulator(
    std::vector<std::unique_ptr<Wavetable>> gOscillators, float waveNumber)
{
    for (unsigned int n = 0; n < wavetable_.size(); n++) {
        wavetable_[n] = prompt_WaveType(waveNumber, static_cast<float>(n));
    }
    auto oscillator = std::make_unique<Wavetable>(sampleRate_, wavetable_, phase_);
    gOscillators.push_back(std::move(oscillator));
    return gOscillators;
}

