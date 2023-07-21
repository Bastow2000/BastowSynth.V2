
#pragma once
#include <JuceHeader.h>

// https://corecoding.com/utilities/rgb-or-hex-to-float.php

// Component Volume Button 1 - 3 ID's
constexpr static int versionHint = 1;
constexpr static int totalNumOscillators = 33;
const static juce::String cvb1Id = "cvb1";
const static juce::String CVB1Name = "CVB1";
const static juce::String cvb2Id = "cvb2";
const static juce::String CVB2Name = "CVB2";
const static juce::String cvb3Id = "cvb3";
const static juce::String CVB3Name = "CVB3";

const static juce::String gain0Id = "gain0Id";
const static juce::String Gain0Name = "Gain0ID";

const static juce::String attackId = "attackId";
const static juce::String AttackName = "AttackId";
const static juce::String decayId = "decayId";
const static juce::String DecayName = "DecayId";
const static juce::String sustainId = "sustainId";
const static juce::String SustainName = "SustainId";
const static juce::String releaseId = "releaseId";
const static juce::String ReleaseName = "ReleaseId";
const static juce::String holdId = "holdId";
const static juce::String HoldName = "HoldId";
const static juce::String decay2Id = "decay2Id";
const static juce::String Decay2Name = "decay2Id";


constexpr static std::string_view baseGainId = "gainId";
constexpr static std::string_view baseGainName = "GainName";

constexpr static std::string_view baseFreqId = "freqId";
constexpr static std::string_view baseFreqName = "FreqName";
constexpr static std::string_view baseWaveTypeId = "waveTypeId";
constexpr static std::string_view baseWaveTypeName = "WaveTypeName";

//Jamie Pond Code 2023 July {
template <typename T>
constexpr static std::string concatenateStringAndInt (T base, int index)
{
    return std::string (base) + std::to_string (index);
}

constexpr static std::string getWaveTypeId (int index)
{
    return concatenateStringAndInt (baseWaveTypeId, index);
}

constexpr static std::string getFreqId (int index)
{
    return concatenateStringAndInt (baseFreqId, index);
}

constexpr static std::string getGainId (int index)
{
    return concatenateStringAndInt (baseGainId, index);
}
//}

namespace C1
{
const juce::Colour red = juce::Colour::fromFloatRGBA (0.91f, 0.25f, 0.25f, 1.0f);
const juce::Colour orange = juce::Colour::fromFloatRGBA (0.91f, 0.62f, 0.25f, 1.0f);
const juce::Colour yellow = juce::Colour::fromFloatRGBA (0.81f, 0.74f, 0.15f, 1.0f);
const juce::Colour brightGreen = juce::Colour::fromFloatRGBA (0.64f, 0.81f, 0.15f, 1.0f);
const juce::Colour limeGreen = juce::Colour::fromFloatRGBA (0.31f, 0.91f, 0.25f, 1.0f);
const juce::Colour brightBlue = juce::Colour::fromFloatRGBA (0.25f, 0.66f, 0.91f, 1.0f);
const juce::Colour blue = juce::Colour::fromFloatRGBA (0.25f, 0.42f, 0.91f, 1.0f);
const juce::Colour violet = juce::Colour::fromFloatRGBA (0.62f, 0.25f, 0.91f, 1.0f);
const juce::Colour magenta = juce::Colour::fromFloatRGBA (0.91f, 0.25f, 0.80f, 1.0f);
const juce::Colour cosmic = juce::Colour::fromFloatRGBA (0.914f, 0.251f, 0.439f, 1.0f);
const juce::Colour grey = juce::Colour::fromFloatRGBA (0.42f, 0.42f, 0.42f, 1.0f);
const juce::Colour black = juce::Colour::fromFloatRGBA (0.0f, 0.0f, 0.0f, 1.0f);
} // namespace C1

namespace C2
{

const juce::Colour red = juce::Colour::fromFloatRGBA (0.616f, 0.286f, 0.286f, 1.0f);
const juce::Colour orange = juce::Colour::fromFloatRGBA (0.714f, 0.549f, 0.349f, 1.0f);
const juce::Colour yellow = juce::Colour::fromFloatRGBA (0.616f, 0.612f, 0.349f, 1.0f);
const juce::Colour brightGreen = juce::Colour::fromFloatRGBA (0.545f, 0.616f, 0.353f, 1.0f);
const juce::Colour limeGreen = juce::Colour::fromFloatRGBA (0.376f, 0.612f, 0.353f, 1.0f);
const juce::Colour brightBlue = juce::Colour::fromFloatRGBA (0.357f, 0.518f, 0.616f, 1.0f);
const juce::Colour blue = juce::Colour::fromFloatRGBA (0.333f, 0.396f, 0.588f, 1.0f);
const juce::Colour violet = juce::Colour::fromFloatRGBA (0.412f, 0.251f, 0.541f, 1.0f);
const juce::Colour magenta = juce::Colour::fromFloatRGBA (0.494f, 0.282f, 0.459f, 1.0f);
const juce::Colour cosmic = juce::Colour::fromFloatRGBA (0.494f, 0.251f, 0.322f, 1.0f);
const juce::Colour grey = juce::Colour::fromFloatRGBA (0.361f, 0.329f, 0.329f, 1.0f);
const juce::Colour white = juce::Colour::fromFloatRGBA (0.f, 0.f, 0.f, 1.0f);
} // namespace C2

namespace C3
{
const juce::Colour red = juce::Colour::fromFloatRGBA (0.569f, 0.f, 0.f, 1.0f);
const juce::Colour orange = juce::Colour::fromFloatRGBA (0.686f, 0.376f, 0.f, 1.0f);
const juce::Colour yellow = juce::Colour::fromFloatRGBA (0.616f, 0.604f, 0.f, 1.0f);
const juce::Colour brightGreen = juce::Colour::fromFloatRGBA (0.427f, 0.584f, 0.f, 1.0f);
const juce::Colour limeGreen = juce::Colour::fromFloatRGBA (0.063f, 0.576f, 0.008f, 1.0f);
const juce::Colour brightBlue = juce::Colour::fromFloatRGBA (0.f, 0.38f, 0.62f, 1.0f);
const juce::Colour blue = juce::Colour::fromFloatRGBA (0.008f, 0.173f, 0.671f, 1.0f);
const juce::Colour violet = juce::Colour::fromFloatRGBA (0.306f, 0.f, 0.545f, 1.0f);
const juce::Colour magenta = juce::Colour::fromFloatRGBA (0.478f, 0.f, 0.396f, 1.0f);
const juce::Colour cosmic = juce::Colour::fromFloatRGBA (0.49f, 0.149f, 0.243f, 1.0f);
const juce::Colour grey = juce::Colour::fromFloatRGBA (0.247f, 0.231f, 0.255f, 1.0f);
const juce::Colour white = juce::Colour::fromFloatRGBA (0.f, 0.f, 0.f, 1.0f);

} // namespace C3

namespace C4
{
const juce::Colour offWhite = juce::Colour::fromFloatRGBA (0.83f, 0.83f, 0.89f, 1.0f);
const juce::Colour white = juce::Colour::fromFloatRGBA (0.9f, 0.9f, 0.9f, 1.0f);
const juce::Colour darkGrey = juce::Colour::fromFloatRGBA (0.16f, 0.15f, 0.15f, 1.0f);
const juce::Colour lightGrey = juce::Colour::fromFloatRGBA (0.74f, 0.76f, 0.76f, 1.0f);
const juce::Colour lightBlack = juce::Colour::fromFloatRGBA (0.13f, 0.13f, 0.13f, 1.0f);
const juce::Colour shadeGrey = juce::Colour::fromFloatRGBA (0.2f, 0.2f, 0.2f, 1.0f);
const juce::Colour lightblack = juce::Colour::fromFloatRGBA (0.01f, 0.01f, 0.01f, 1.0f);

} // namespace C4

#define gainIdM "gainIdM"
#define GainNameM "GainNameM"

#define gainIdM "gainIdM"
#define GainNameM "GainNameM"

