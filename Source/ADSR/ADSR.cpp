#include "ADSR.h"

#include "ADSR.h"

// CustomADSR constructor
CustomADSR::CustomADSR()
{
    // Initialize default values
    attackTime_ = 0.001;
    attackHoldTime_ = 0.001;
    holdTime_ = attackHoldTime_;
    decayTime_ = 0.001;
    decay2Time_ = 0.001;
    sustainLevel_ = 1;
    decay2Level_ = 0;
    releaseTime_ = 0.001;

    state_ = StateOff;
    isHolding_ = false;
}

// Set the sample rate
void CustomADSR::setSampleRate(float rate)
{
    ramp_.setSampleRate(rate);
}

// Set the attack time
void CustomADSR::setAttackTime(float attackTime)
{
    attackTime_ = (attackTime >= 0) ? attackTime : 0;
}

// Set the attack hold time
void CustomADSR::setAttackHoldTime(float attackHoldTime)
{
    attackHoldTime_ = (attackHoldTime >= 0) ? attackHoldTime : 0;
    holdTime_ = attackHoldTime_;
}

// Set the decay time
void CustomADSR::setDecayTime(float decayTime)
{
    decayTime_ = (decayTime >= 0) ? decayTime : 0;
}

// Set the second decay time
void CustomADSR::setDecay2Time(float decay2Time)
{
    decay2Time_ = (decay2Time >= 0) ? decay2Time : 0;
}

// Set the sustain level
void CustomADSR::setSustainLevel(float sustainLevel)
{
    sustainLevel_ = (sustainLevel < 0) ? 0 : ((sustainLevel > 1) ? 1 : sustainLevel);
}

// Set the release time
void CustomADSR::setReleaseTime(float releaseTime)
{
    releaseTime_ = (releaseTime >= 0) ? releaseTime : 0;
}

// Reset all parameters to their initial values
void CustomADSR::reset()
{
    attackTime_ = 0.0f;
    decayTime_ = 0.0f;
    sustainLevel_ = 0.0f;
    releaseTime_ = 0.0f;
    decayTimeSegment_ = 0.0f;
    decayLevelSegment_ = 0.0f;
    releaseTimeSegment_ = 0.0f;
    releaseLevelSegment_ = 0.0f;
    holdTime_ = 0.0f;
    attackHoldTime_ = 0.0f;
    decay2Time_ = 0.0f;
    decay2Level_ = 0.0f;
    releaseLevel_ = 0.0f;
    overshoot_ = 0.0f;
    isHolding_ = false;
    state_ = StateOff;
}

// Trigger the ADSR envelope
void CustomADSR::trigger()
{
    state_ = StateAttack;
    ramp_.rampTo(1.0, attackTime_);

    if (state_ != StateAttack) {
        isHolding_ = false;
    }
    else {
        isHolding_ = true;
    }
}

// Hold the ADSR envelope
void CustomADSR::hold()
{
    if ((state_ == StateAttack || state_ == StateHold) && attackHoldTime_ > 0) {
        isHolding_ = true;
    }
    else if (state_ == StateDecay || state_ == StateSustain) {
        state_ = StateDecay2;
        ramp_.rampTo(sustainLevel_, decay2Time_);
    }
}

// Release the ADSR envelope
void CustomADSR::release()
{
    if (state_ == StateAttack || state_ == StateHold)
    {
        state_ = StateRelease;
        ramp_.rampTo(releaseLevel_, releaseTime_);

        isHolding_ = false;
    }
    else if (state_ == StateDecay || state_ == StateDecay2 || state_ == StateSustain) {
        state_ = StateRelease;
        ramp_.rampTo(0.0f, releaseTime_);

        isHolding_ = false;
    }
}

// Set the segment time and level for a specific segment type
void CustomADSR::setSegment(float segmentTime, float segmentLevel, SegmentType segmentType)
{
    switch (segmentType)
    {
        case Decay:
            decayTimeSegment_ = segmentTime;
            decayLevelSegment_ = segmentLevel;
            break;
        case Decay2:
            decay2Time_ = segmentTime;
            decay2Level_ = segmentLevel;
            break;
        case Release:
            releaseTime_ = segmentTime;
            releaseLevel_ = segmentLevel;
            break;
    }
}

// Process the ADSR envelope and return the current value
float CustomADSR::process()
{
    switch (state_) {
        case StateOff:
            break;

        case StateAttack:
            if (ramp_.finished()) {
                if (isHolding_) {
                    state_ = StateHold;
                }
                else {
                    state_ = StateDecay;
                    ramp_.rampTo(sustainLevel_, decayTime_);
                }
            }
            break;

        case StateHold:
            if (isHolding_) {
                state_ = StateDecay;
                ramp_.rampTo(sustainLevel_, decayTime_);
            }
            break;

        case StateDecay:
            if (ramp_.finished()) {
                state_ = StateSustain;
            }
            break;

        case StateSustain:
            if (isHolding_) {
                state_ = StateDecay2;
                ramp_.rampTo(sustainLevel_, decayTime_);
            }
            break;

        case StateDecay2:
            if (ramp_.finished()) {
                state_ = StateRelease;
                ramp_.rampTo(0.0f, releaseTime_);
            }
            break;

        case StateRelease:
            if (ramp_.finished()) {
                state_ = StateOff;
            }
            break;
    }

    return ramp_.process();
}

// Check if the ADSR envelope is active
bool CustomADSR::isActive()
{
    return (state_ != StateOff);
}

// CustomADSR destructor
CustomADSR::~CustomADSR()
{
    // Destructor
    // Nothing to do here
}