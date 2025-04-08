#pragma once

#include <JuceHeader.h>
#include <arm_neon.h>
#include <cmath>
#include "ADSR.h"
#include <vector>

class Wavetable
{
public:
    // Default Constructor
    Wavetable() {}

    // Default Destructor
    ~Wavetable() {wavetable_.clear();};

    // Additional Constructor with arguments
    Wavetable (float sampleRate, std::vector<float>& wavetable, float phase);

    void setSampleRate (float newSampleRate);
    void updatePhaseIncrement();

    // Sets up important variables used in the classes functions
    void setup (float sampleRate, std::vector<float>& wavetable, float phase);

    // Sets the frequency for the <Wavetable*> thats called
    void setFrequency (float f);

       // Implement the function to set the ADSR parameters to your oscillator logic
    void setCustomADSRParameters(float att, float dec, float sus, float rel, float hold, float dec2);

    // Sets new Wavetable
  void setNewWavetable (std::vector<float>& newWavetable);

    // Sets the Amplitude for the <Wavetable*> thats called
    void setAmplitude (float a);

    // Gets the frequency for the <Wavetable*> thats called
    inline float getFrequency() const;

    // Gets the amplitude for the <Wavetable*> thats called
   inline float getAmplitude() const;

    // Takes wavetable from the Constructor reads through the buffer
    float process();

    // Takes wavetable from the Constructor and applies linearInterpolation
    inline float linearInterpolation() const;

    float cubicInterpolation();
  void setNewWavetable(std::vector<float>&& newTable, float sampleRate);

private:
    // Buffer holding the wavetable
    std::vector<float> wavetable_;

  juce::SpinLock spinLock_;
    // 1 divided by the audio sample rate
    float inverseSampleRate_;

    float sampleRate_;

    float frequency_;

    float amplitude_;

    float readPointer_;

    float phaseIncrement_;

    float phase_;
    CustomADSR amplitudeADSR;
};
