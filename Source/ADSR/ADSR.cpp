#include "ADSR.h"

// Constructor. Set up some default parameters.
// We can also use initialisation lists before the 
// start of the curly braces to set these values

    CustomADSR::CustomADSR()
    {
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

		void CustomADSR::setSampleRate(float rate)
	{
		ramp_.setSampleRate(rate);
	}

	void CustomADSR::setAttackTime(float attackTime)
	{
		attackTime_ = (attackTime >= 0) ? attackTime : 0;
	}

	void CustomADSR::setAttackHoldTime(float attackHoldTime)
	{
		attackHoldTime_ = (attackHoldTime >= 0) ? attackHoldTime : 0;
		holdTime_ = attackHoldTime_;
	}

	void CustomADSR::setDecayTime(float decayTime)
	{
		decayTime_ = (decayTime >= 0) ? decayTime : 0;
	}

	void CustomADSR::setDecay2Time(float decay2Time)
	{
		decay2Time_ = (decay2Time >= 0) ? decay2Time : 0;
	}

	void CustomADSR::setSustainLevel(float sustainLevel)
	{
		sustainLevel_ = (sustainLevel < 0) ? 0 : ((sustainLevel > 1) ? 1 : sustainLevel);
	}

	void CustomADSR::setReleaseTime(float releaseTime)
	{
		releaseTime_ = (releaseTime >= 0) ? releaseTime : 0;
	}

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

    void CustomADSR::trigger()
    {
         state_ = StateAttack;
		ramp_.rampTo(1.0, attackTime_);

		if (state_ != StateAttack) {
			isHolding_ = false; // Set isHolding_ to false if not StateAttack
		}
		else {
			isHolding_ = true;
    	}
    }

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

    void CustomADSR::release()
    {
		if (state_ == StateAttack || state_ == StateHold) 
		{
        state_ = StateRelease;
        ramp_.rampTo(releaseLevel_, releaseTime_);

        isHolding_ = false;
		}
		else if (state_ == StateDecay || state_ == StateDecay2 || state_ == StateSustain)state_ = StateRelease;
			ramp_.rampTo(0.0f, releaseTime_);

			isHolding_ = false;
		
    }


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


    float CustomADSR::process()
	{
		switch (state_) {
			case StateOff:
				// Nothing to do here. trigger() will change the state.
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

    bool CustomADSR::isActive()
    {
        return (state_ != StateOff);
    }

// Destructor
CustomADSR::~CustomADSR() 
{
	// Nothing to do here
}
/*
CustomADSR::CustomADSR()
{
    attackTime_ = 0.001;
    attackHoldTime_ = 0.001;
    decayTime_ = 0.001;
    decay2Time_ = 0.001;
    sustainLevel_ = 1;
    decay2Level_ = 0;
    releaseTime_ = 0.001;

    state_ = StateOff;
    isHolding_ = false;
}

void CustomADSR::setSampleRate(float rate)
{
    ramp_.setSampleRate(rate);
}

void CustomADSR::setAttackTime(float attackTime)
{
    attackTime_ = (attackTime >= 0) ? attackTime : 0;
}

void CustomADSR::setAttackHoldTime(float attackHoldTime)
{
    attackHoldTime_ = (attackHoldTime >= 0) ? attackHoldTime : 0;
    holdTime_ = attackHoldTime_;
}

void CustomADSR::setDecayTime(float decayTime)
{
    decayTime_ = (decayTime >= 0) ? decayTime : 0;
}

void CustomADSR::setDecay2Time(float decay2Time)
{
    decay2Time_ = (decay2Time >= 0) ? decay2Time : 0;
}

void CustomADSR::setSustainLevel(float sustainLevel)
{
    sustainLevel_ = (sustainLevel < 0) ? 0 : ((sustainLevel > 1) ? 1 : sustainLevel);
}

void CustomADSR::setReleaseTime(float releaseTime)
{
    releaseTime_ = (releaseTime >= 0) ? releaseTime : 0;
}

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

void CustomADSR::trigger()
{
    if (state_ == StateAttack)
    {
        isHolding_ = true;
    }
    else
    {
        state_ = StateAttack;
        ramp_.rampTo(1.0, attackTime_);
        isHolding_ = false;
    }
}

void CustomADSR::hold()
{
    if ((state_ == StateAttack || state_ == StateHold) && attackHoldTime_ > 0)
    {
        isHolding_ = true;
    }
    else if (state_ == StateDecay || state_ == StateSustain)
    {
        state_ = StateDecay2;
        ramp_.rampTo(sustainLevel_, decay2Time_);
    }
}

void CustomADSR::release()
{
    if (state_ == StateAttack || state_ == StateHold)
    {
        state_ = StateRelease;
        ramp_.rampTo(releaseLevel_, releaseTime_);
        isHolding_ = false;
    }
    else if (state_ == StateDecay || state_ == StateDecay2 || state_ == StateSustain)
    {
        state_ = StateRelease;
        ramp_.rampTo(0.0f, releaseTime_);
        isHolding_ = false;
    }
}

float CustomADSR::process()
{
    switch (state_)
    {
        case StateOff:
            // Nothing to do here. trigger() will change the state.
        case StateAttack:
 			if (ramp_.finished())
            {
                if (isHolding_)
                {
                    state_ = StateHold;
                }
                else
                {
                    state_ = StateDecay;
                    ramp_.rampTo(sustainLevel_, decayTime_);
                }
            }
            break;
        case StateHold:
            if (isHolding_)
            {
                state_ = StateDecay;
                ramp_.rampTo(sustainLevel_, decayTime_);
            }
            break;
        case StateDecay:
            if (ramp_.finished())
            {
                state_ = StateSustain;
            }
            break;
        case StateSustain:
            if (isHolding_)
            {
                state_ = StateDecay2;
                ramp_.rampTo(decay2Level_, decay2Time_);
            }
            break;
        case StateDecay2:
            if (ramp_.finished())
            {
                state_ = StateRelease;
                ramp_.rampTo(0.0f, releaseTime_);
            }
            break;
        case StateRelease:
            if (ramp_.finished())
            {
                state_ = StateOff;
            }
            break;
        default:
            break; // Handle unexpected states
    }

    return ramp_.process();
}

bool CustomADSR::isActive()
{
    return (state_ != StateOff);
}

// Destructor
CustomADSR::~CustomADSR()
{
    // Nothing to do here
}*/