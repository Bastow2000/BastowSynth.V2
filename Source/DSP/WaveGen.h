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
    ~GenerateWavetable() { wavetable_.clear(); }

    // Additional Constructor with arguments
    GenerateWavetable (float sampleRate, std::vector<float>& wavetable, float phase, int midiNoteNumber, int wavetype);

    // Sets up important variables used in the classes functions
    void setup (float sampleRate, std::vector<float>& wavetable, float phase, int midiNoteNumber, int wavetype);

    // When applies makes a wave semi bandlimited
    inline float poly_blep (float t, float dt);

    // Creates the wave type and sets it to the wavetable
    inline float prompt_WaveType ( float n);

    inline double generate_Triangle(float n);

    inline double generate_Square(float n);

    inline double generate_Sawtooth(float n);

    inline double lanczosFactor(float numHarmIndex);

   

    // Used to create modulators
    std::vector<Wavetable*> prompt_Modulator (std::vector<Wavetable*> gOscillators, unsigned int waveNumber);

    // Used to create carriers
    std::vector<float> prompt_Carrier ();

private:
    // Buffer holding the wavetable
    std::vector<float> wavetable_;

    int midiNoteNumber_;

    // 1 divided by the audio sample rate
    float inverseSampleRate_;

    float sampleRate_;

    unsigned int numHarmonics_;

    unsigned int waveType_;

    float phaseIncrement_;

    float pitchConvertHz_;

    float phase_;
};
