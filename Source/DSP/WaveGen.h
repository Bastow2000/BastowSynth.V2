#pragma once

#include <vector>

#include "WaveSetup.h"
#include <JuceHeader.h>
#include <arm_neon.h>
#include <cmath>

class GenerateWavetable
{
public:
    // Default Constructor
    GenerateWavetable() {}

    // Default Destructor
    ~GenerateWavetable() {wavetable_.clear();};;

    // Additional Constructor with arguments
    GenerateWavetable (float sampleRate, std::vector<float> wavetable, float phase);

    // Sets up important variables used in the classes functions
    void setup (float sampleRate, float phase);

    // When applies makes a wave semi bandlimited
    inline float poly_blep (float t, float dt);

    // Creates the wave type and sets it to the wavetable
    inline float prompt_WaveType (unsigned int waveNumber, float n);

    // Used to create modulators
    std::vector<std::unique_ptr<Wavetable>> prompt_Modulator (std::vector<std::unique_ptr<Wavetable>> gOscillators, unsigned int waveNumber);

    // Used to create carriers
    std::vector<float> prompt_Harmonics (unsigned int waveNumber);

private:

    // Buffer holding the wavetable
    std::vector<float> wavetable_;

    // 1 divided by the audio sample rate
    float inverseSampleRate_;

    float sampleRate_;

    float phaseIncrement_;

    float phase_;
};
