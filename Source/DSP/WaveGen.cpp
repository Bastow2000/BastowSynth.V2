#include "WaveGen.h"
// Constructor taking arguments for sample rate, wavetable data and phase
GenerateWavetable::GenerateWavetable (float sampleRate, std::vector<float>& wavetable, float phase, int midiNoteNumber, int wavetype)
{
    setup (sampleRate, wavetable, phase, midiNoteNumber, wavetype);
}

// Setting up wavetable data, phase and sampleRate to be used in functions & their calls
void GenerateWavetable::setup (float sampleRate, std::vector<float>& wavetable, float phase, int midiNoteNumber, int wavetype)
{
    // It's faster to multiply than to divide on most platforms, so we save the inverse
    // of the sample rate for use in the phase calculation later
    inverseSampleRate_ = 1.0f / sampleRate;

    // Assign parameters
    sampleRate_ = sampleRate;

    wavetable_ = wavetable;

    waveType_ = wavetype;

    midiNoteNumber_ = midiNoteNumber;

    float pitchConvertHz = 440.0f * std::pow(2.0f, (midiNoteNumber_ - 69) / 12.0f);
    // C8 108 4186 Hz can have 5 harmonics
    // C2 36 35 34 33 32 31 30 29
    // C1 29 43 Hz Can have 513 harmoncics
    // Calculate the number of harmonics based on the fundamental frequency (pitchHz)
    unsigned int numHarmonics = static_cast<unsigned int>((sampleRate /  2) / pitchConvertHz);

    numHarmonics_ = numHarmonics /** 0.5 */; // Control the number of harmonics for the harmonic sequence
    pitchConvertHz_ = pitchConvertHz;

    phase_ = phase;

    // Calculates the phase increment for a waveform table of a given size and sample rate
    phaseIncrement_ = (wavetable_.size() * inverseSampleRate_);
}

// Calculates poly_blep (Martin Finke,(no date))
float GenerateWavetable::poly_blep (float t /*phase*/, float dt /*phaseHalfIncrement*/)
{
     // Adjusting the range of t from [0, 1] to [-1, 1]
    t = 2.0f * t - 1.0f;

    if (t >= -1.0f && t <= 0.0f) 
    {
        // Applying the formula for the range [-1, 0]
        return std::pow(t,2) + 2.0f * t + 1.0f;
    } 
    else if (t >= 0.0f && t <= 1.0f) 
    {
        // Applying the formula for the range [0, 1]
        return 2.0f * t - std::pow(t,2) - 1.0f;
    } 
    else 
    {
        // Out of range
        return 0.0f;
    }
}

double GenerateWavetable::lanczosFactor(float numHarmIndex)
{
    // Gibbs phenomanon Lanczos factor
    // Reduces ripples in wave form
    double x = numHarmIndex * M_PI / numHarmonics_;
    double sigma = (x == 0) ? 1.0 : sin(x) / x;
    return sigma;
}

double GenerateWavetable::generate_Triangle(float n)
{
    double value = 0.0;
    for (unsigned int numHarm = 1; numHarm <= numHarmonics_; numHarm += 2) 
    {
        value += pow(-1, numHarm) * (1.0f / pow((2 * numHarm + 1), 2)) * 
        lanczosFactor(numHarm) * sin(2.0f * M_PI * (2 *numHarm + 1)* n / wavetable_.size());
    }
    return value;
}
      
double GenerateWavetable::generate_Square(float n)
{
    double value = 0.0;
    for (unsigned int numHarm = 1; numHarm <= numHarmonics_; numHarm +=2) 
    {
        
        value +=  (1.0f /(2 *numHarm +1)) * 
        lanczosFactor(numHarm) * sin(2.0f * M_PI * (2 *numHarm + 1)* n / wavetable_.size()); 
    }
    return value;
}

double GenerateWavetable::generate_Sawtooth(float n)
{
    double value = 0.0;
    for (unsigned int numHarm = 1; numHarm <= numHarmonics_; ++numHarm)
    {
        value +=  pow(-1, numHarm + 1) * (1.0f / numHarm) * 
        lanczosFactor(numHarm) * sin(2.0f * M_PI * numHarm * n / wavetable_.size()); 
    }
    return value;
}

float GenerateWavetable::prompt_WaveType (float n)
{
    // Set values for poly_blep
    float dt = phaseIncrement_ / wavetable_.size();

    float t = phase_;

    // Initalise value for further use
    float value = 0.0f;

    // Use argument waveNumber to change the type of the wave
    switch (waveType_)
    {
        case 1: // Sine Wave
            // The simplest waveform; a single frequency with no harmonics.
            // It requires just one term of its Fourier series.
            value = (sinf(2.0f *(float) M_PI * (float)n  / (float)wavetable_.size() + phase_));
            break;
        case 2: // Triangle Wave
            // Triangle waves are made up of odd harmonics (1, 3, 5, ...),
            // and the amplitude of each harmonic is the square of its number.
            // The series alternates in sign (+, -, +, -).
            value += (float)generate_Triangle(n) + phase_;
            break;
        case 3: // Square Wave
            // Square waves contain only odd harmonics (1, 3, 5, ...),
            // and the amplitude of each harmonic decreases in proportion to its number.
            // This creates a timbre that is rich in higher frequencies.
            value += (float)generate_Square(n) + phase_;
            break;
        case 4: // Sawtooth Wave
            // Sawtooth waves include all harmonics (1, 2, 3, ...),
            // and similar to the square wave, the amplitude of each harmonic
            // decreases in proportion to its number. However, the sawtooth wave
            // does not skip even harmonics.
            value += (float)generate_Sawtooth(n) + phase_;
            break;
    }
    // Return the wave type and Subtract poly_blep
    return value -= poly_blep (t, dt);
}

// Uses the wavetable_ defined from the constuctor to assign a wave type to wavetable_ then adds the
// wavetable_ to <Wavetable*> and sends the <Wavetable*> back to render to be used
std::vector<Wavetable*> GenerateWavetable::prompt_Modulator (std::vector<Wavetable*> gOscillators, unsigned int waveNumber)
{
    for (unsigned int n = 0; n < wavetable_.size(); n++)
    {
        wavetable_[n] = prompt_WaveType (n);
    }
    Wavetable* oscillators = new Wavetable (sampleRate_, wavetable_, phase_);

    gOscillators.push_back (oscillators);

    // Delete memory without -1 there is a bus error
    for (unsigned int i = 0; i < gOscillators.size(); i++)
    {
        delete gOscillators[i];
    }

    return gOscillators;
}

// Uses the wavetable_ defined from the constuctor to assign a wave type to wavetable_ then returns value
std::vector<float> GenerateWavetable::prompt_Carrier ()
{
    for (unsigned int n = 0; n < wavetable_.size(); n++)
    {
        wavetable_[n] = prompt_WaveType (n);
    }
    return wavetable_;
}
