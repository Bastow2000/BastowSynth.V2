#pragma once

#include "Ramp.h"
// Qmul Lecture Class code modified
enum SegmentType {
  Decay,
  Decay2,
  Release
};
class CustomADSR {
private:
	// ADSR state machine variables, used internally
	enum State {
		 StateOff,
        StateAttack,
        StateHold,
        StateDecay,
        StateSustain,
        StateDecay2,
        StateRelease
	};

public:
	// Constructor
	CustomADSR();
	
	// Constructor with argument
	CustomADSR(float sampleRate);
	
	// Set the sample rate, used for all calculations
	void setSampleRate(float rate);

	void reset();
	
	// Start the envelope, going to the Attack state
	void trigger();
	
	// Stop the envelope, going to the Release state
	void release();
	
	// Calculate the next sample of output, changing the envelope
	// state as needed
	float process(); 
	
	// Indicate whether the envelope is active or not (i.e. in
	// anything other than the Off state
	bool isActive();
	
	// Methods for getting and setting parameters
	float getAttackTime() { return attackTime_; }
	float getDecayTime() { return decayTime_; }
	float getHoldTime() { return holdTime_; }
	float getSustainLevel() { return sustainLevel_; }
	float getDecay2Time() { return decay2Time_; }
	float getReleaseTime() { return releaseTime_; }
	
	void setAttackTime(float attackTime);
	void setAttackHoldTime(float attackHoldTime);
	void hold();
	void setDecayTime(float decayTime);	
	void setSustainLevel(float sustainLevel);
	void setReleaseTime(float releaseTime);
	void setDecay2Time(float decay2Time);
	void setSegment(float segmentTime, float segmentLevel, SegmentType segmentType);
	
	
	
	// Destructor
	~CustomADSR();

private:
	// State variables and parameters, not accessible to the outside world
	float attackTime_;
	float decayTime_;
	float sustainLevel_;
	float releaseTime_;
    float decayTimeSegment_;
    float decayLevelSegment_;
    float releaseTimeSegment_;
    float releaseLevelSegment_;
	float holdTime_;
	float attackHoldTime_;
	
	float decay2Time_;
	float decay2Level_;
	//float releaseTime_;
	float releaseLevel_;
	float overshoot_;
	bool isHolding_; // to keep track if we are in the Hold phase
	
	State state_;			// Current state of the ADSR (one of the enum values above)
	Ramp ramp_;				// Line segment generator
};